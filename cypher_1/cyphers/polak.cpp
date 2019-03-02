
#include <juce_graphics/juce_graphics.h>
#include <boost/filesystem.hpp>

namespace 
{
    struct Glyph
    {
        // Borders
        bool left   : 1;
        bool right  : 1;
        bool top    : 1;
        bool bottom : 1;

        // Dots
        std::vector<char> dots;
    };

    // Pre-generate the alphabet to simplify workarounds for 'ch'
    std::vector<std::string> generate_alphabet ()
    {
        std::vector<std::string> result;

        for (char c = 'a'; c <= 'z'; ++c)
        {
            result.push_back(std::string(1, c));

            if (c == 'h')
                result.push_back("ch");
        }

        return result;
    }

    const std::vector<std::string> alphabet = generate_alphabet();

    Glyph get_glyph (const std::string& c)
    {
        // Find the letter in the alphabet
        auto it = std::find(alphabet.begin(), alphabet.end(), boost::to_lower_copy(c));

        if (it == alphabet.end()) {
            throw std::runtime_error("Can't translate '" + c + "'");
        }

        auto index = it - alphabet.begin();
        auto row = index / 9;
        auto col = index % 9;

        Glyph g;
        g.left = col > 2;
        g.right = col < 6;
        g.top = row > 0;
        g.bottom = row < 2;
        
        g.dots.push_back(index % 3);

        return g;
    }

    Glyph next_glyph (const std::string& s, std::size_t& offset)
    {
        // Workaround for the idiotic 'letter' CH
        auto size = boost::istarts_with(s.c_str() + offset, "ch") ? 2 : 1;
        offset += size;
        return get_glyph(s.substr(offset - size, size));
    }

    enum glyph_metrics : int
    {
        LINE = 3,
        DOT_R = 6,
        SPACING = 7,
        GLYPH_SPACING = 10,
        CHARACTER_WIDTH = 17,
        CHARACTER_FONT_SIZE = 30,

        // Calculated values
        DOT_D = DOT_R*2,
        HEIGHT = LINE*2 + DOT_D + SPACING *2,
        WIDTH = LINE*2 + DOT_D*3 + SPACING*4,

    };

    void draw_glyph (float x, float y, const Glyph glyph, juce::Graphics& graphics)
    {
        std::vector<juce::Line<float>> lines;
        const float lineOffsetCenter = glyph_metrics::LINE / 2.0f;


        if (glyph.left) {
            lines.push_back({x + lineOffsetCenter, y, x + lineOffsetCenter, y + HEIGHT});
        }

        if (glyph.right) {
            lines.push_back({x - lineOffsetCenter + WIDTH, y, x - lineOffsetCenter + WIDTH, y + HEIGHT});
        }

        if (glyph.top) {
            lines.push_back({x, y + lineOffsetCenter, x + WIDTH, y + lineOffsetCenter});
        }

        if (glyph.bottom) {
            lines.push_back({x, y - lineOffsetCenter + HEIGHT, x + WIDTH, y - lineOffsetCenter + HEIGHT});
        }

        for (auto& line: lines) {
            graphics.drawLine(line, LINE);
        }

        for (auto dot: glyph.dots)
        {
            graphics.fillEllipse(x + LINE + SPACING + dot*(DOT_D + SPACING), y + LINE + SPACING, DOT_D, DOT_D);
        }
    }

    void draw_char (float x, float y, char c, juce::Graphics& graphics)
    {
        graphics.drawText(juce::String(&c, 1), juce::Rectangle<float>(x, y, CHARACTER_WIDTH, HEIGHT), juce::Justification::centred);
    }
}

DEFINE_BINARY_CYPHER(polak, "polak; Uses an obscure pigpen variant, known somewhat cryptically as 'velky polsky kriz' (written in an unknown language believed to exist in central europe around the 21st century);"
                            "int w = 1240 #Width of the output image (by default A4 @ 150 DPI);"
                            "int h = 1754 #Height of the output image (by default A4 @ 150 DPI);"
                            "int vertical = 30 #Vertical spacing between individual lines (adjust this if you're changing the DPI);"
    )
{
    int width = parameters.get<int>("w");
    int height = parameters.get<int>("h");
    int vertical = parameters.get<int>("vertical");

    juce::Image canvas (juce::Image::RGB, width, height, false);
    juce::Graphics graphics (canvas);
    
    graphics.fillAll(juce::Colours::white);
    graphics.setColour(juce::Colours::black);
    graphics.setFont(CHARACTER_FONT_SIZE);

    std::size_t sourceOffset = 0;
    
    int x = GLYPH_SPACING;
    int y = GLYPH_SPACING;

    bool newline = false; // Start with false, even if we are on a new line (in case there is a deliberate indentation)

    while (sourceOffset < source.size())
    {
        if (source[sourceOffset] == ' ')
        {
            if (newline)
            {
                // Ignore spaces at the beginning of a new line
                ++sourceOffset;
                continue;
            }
        }

        newline = false;

        if (isalpha(source[sourceOffset]))
        {
            auto glyph = next_glyph(source, sourceOffset);
            draw_glyph(float(x), float(y), glyph, graphics);

            x += WIDTH + GLYPH_SPACING;
        }
        else
        {
            draw_char(float(x), float(y), source[sourceOffset], graphics);
            ++sourceOffset;

            x += CHARACTER_WIDTH + GLYPH_SPACING;
        }

        if (canvas.getWidth() - x < WIDTH + GLYPH_SPACING)
        {
            x = GLYPH_SPACING;
            y += HEIGHT + vertical;
            newline = true;
        }
    }

    juce::MemoryOutputStream mos;

    std::string outputFormat;
    if (auto outputFile = cypher::utility::program_options().get('o', "output-file"))
    {
        boost::filesystem::path path (outputFile.as<std::string>());
        if (path.has_extension())
        {
            outputFormat = boost::to_lower_copy(path.extension().string());
            boost::trim_left_if(outputFormat, [](char c) { return c == '.'; });
        }
    }

    if (outputFormat == "png")
    {
        juce::PNGImageFormat writer;
        writer.writeImageToStream(canvas, mos);
    }
    else if (outputFormat == "jpg" || outputFormat == "jpeg")
    {
        juce::JPEGImageFormat writer;
        writer.writeImageToStream(canvas, mos);
    }
    else if (outputFormat == "gif")
    {
        juce::GIFImageFormat writer;
        writer.writeImageToStream(canvas, mos);
    }
    else
    {
        std::clog << "Unknown extension \"" << outputFormat << "\", defaulting to PNG\n";
        juce::PNGImageFormat pngWriter;
        pngWriter.writeImageToStream(canvas, mos);
    }

    return std::string(reinterpret_cast<const char*>(mos.getData()), mos.getDataSize());
}