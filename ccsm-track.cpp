#include <iostream>
#include <cxxopts.hpp>
#include <SFML/Graphics.hpp>


//compile: g++ -c ccsm-track.cpp
//then: g++ ccsm-track.o -o ccsm-track -lsfml-graphics -lsfml-window -lsfml-system

using namespace std;
using namespace sf;

typedef vector<Vector2f> Track;

cxxopts::ParseResult parseTrackingArguments(int argc, char **argv) {
    cxxopts::Options options("ccsm-track", "Carbon Copy Signature Matching - record a specific signature by tracking the mouse movement");
    try {
        options.add_options()
                ("h,help", "Print help")
                ("n,name", "Signature name", cxxopts::value<string>());

        options.parse_positional("name");

        auto result = options.parse(argc, argv);

        if (result.count("help")) {
            cout << options.help({"", "Group", "Data", "Training", "Network", "Replication"}) << endl;
            exit(0);
        }
        if (!result.count("name")) {
            cout << "error parsing options: Argument missing: " << " --name" << endl;
            exit(1);
        }
        return result;

    } catch (const cxxopts::OptionException& e)
    {
        cout << "error parsing options: " << e.what() << endl;
        exit(1);
    }

}

void displayDefaultText(RenderWindow &window, Font &font) {
    sf::Text result_prompt;
    result_prompt.setFont(font);
    result_prompt.setCharacterSize(14);
    result_prompt.setPosition(0, 0);
    result_prompt.setString("Left Mouse Button: Track Mouse Movement\nRight Mouse Button: Confirm Signature\nESC: Discard Signature");
    result_prompt.setFillColor(sf::Color::Cyan);
    window.draw(result_prompt);
    window.display();
}

int main(int argc, char** argv) {
    parseTrackingArguments(argc, argv);
    string path = "./signatures/";

    RenderWindow window(VideoMode(500, 500), "Tracking Signatures!");
    window.clear(Color::Black);
    window.display();

    Font font;
    if (!font.loadFromFile("./UbuntuMono-B.ttf"))
    {
        std::cout << "Error loading font\n" ;
    }

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                //TODO save results
                window.close();
            }

            window.clear(sf::Color::Black);
            displayDefaultText(window, font);
        }
    }

}
