#include <iostream>
#include <cxxopts.hpp>
#include <SFML/Graphics.hpp>
#include <fstream>
#include "TrackPad.h"

using namespace std;
using namespace sf;

cxxopts::ParseResult parseViewArguments(int argc, char **argv) {
    cxxopts::Options options("ccsm-view", "Carbon Copy Signature Matching - view signature tracks for a specific name / ");
    try {
        options.add_options()
                ("h,help", "Print help")
                ("n,name", "Signature name", cxxopts::value<string>())
                ("i,num", "Track number to be loaded", cxxopts::value<int>());

        options.parse_positional("name", "num");

        auto result = options.parse(argc, argv);

        if (result.count("help")) {
            cout << options.help({"", "Group", "Data", "Training", "Network", "Replication"}) << endl;
            exit(0);
        }
        if (!result.count("name")) {
            cout << "rms_error parsing options: Argument missing: " << " --name" << endl;
            exit(1);
        }
        return result;

    } catch (const cxxopts::OptionException& e)
    {
        cout << "rms_error parsing options: " << e.what() << endl;
        exit(1);
    }

}

int main(int argc, char** argv) {
    auto args = parseViewArguments(argc, argv);
    string path = "./signatures/";


    /*
     * Window setup
     */
    string window_name = "View Signatures for " + args["name"].as<string>();


    RenderWindow window(VideoMode(1000, 1000), window_name);
    TrackPad trackPad(window, "Left Mouse Button    |  Right Mouse Button  |  ESC (yet to be implemented)\n"
                              "> Next               | > Previous           | > Discard Signature");

    /*
     * Load tracks from file
     */

    path += args["name"].as<string>() + ".sgn.tsv";
    cout << "Load tracks from " << path << endl;
    if (!trackPad.loadTrackListFromFile(path)) {
        cout << "Loading Error: Tracklist cannot be loaded." << endl;
        return 1;
    }

    int num = 0;
    if (args.count("num")) {
        num = args["num"].as<int>();
    }

    if (!trackPad.setActiveTrackByNumber(num)) {
        cout << "Warning: Could not display track no. " << num << ". Out of range!" << endl;
        cout << "Display first track instead." << endl;
        trackPad.setActiveTrackByNumber(0);
    }

    trackPad.drawCurrentTrack();


    /*
     * With open window
     */
    cout << "Launch ccsm-view application" << endl;
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case Event::Closed: {
                    window.close();
                    break;
                }
                case Event::MouseButtonPressed: {
                    if (event.mouseButton.button == Mouse::Left) {
                        if(trackPad.next()) {
                            trackPad.drawCurrentTrack();
                        }
                    } else if (event.mouseButton.button == Mouse::Button::Right) {
                        if(trackPad.previous()) {
                            trackPad.drawCurrentTrack();
                        }
                    }
                    break;
                }
                case Event::KeyPressed: {
                    if (event.key.code == Keyboard::Escape) {
                        //TODO trackPad.discardTrack();
                    }
                    break;
                }
                default:
                    break;
            }
        }
    }

}