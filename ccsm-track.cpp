#include <iostream>
#include <cxxopts.hpp>
#include <SFML/Graphics.hpp>
#include <fstream>
#include "TrackPad.h"

//compile: g++ -c ccsm-track.cpp
//then: g++ ccsm-track.o -o ccsm-track -lsfml-graphics -lsfml-window -lsfml-system

using namespace std;
using namespace sf;




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

bool saveTrackListToFile(vector<Track> &trackList, string &file_path) {
    if (trackList.empty()) {
        cout << "No records are saved: Empty Track List" << endl;
        return false;
    }
    cout << "Saving " << trackList.size() << " signature tracks to: " << file_path << endl;
    fstream file(file_path, ios::app);
    if (!file.good()) {
        cout << file_path << " does not work out" << endl;
        return false;
    }
    for (Track track : trackList) {
        for (Vector2f point : track) {
            file << point.x << " " << point.y << " ";
        }
        file << endl;
    }
    cout << "done successfully" << endl;
    return true;
}

int main(int argc, char** argv) {
    auto args = parseTrackingArguments(argc, argv);
    string path = "./signatures/";


    /*
     * Window setup
     */
    string window_name = "Track Signatures for " + args["name"].as<string>();

    RenderWindow window(VideoMode(1000, 1000), window_name);
    window.setFramerateLimit(120);
    TrackPad trackPad(window, "Left Mouse Button    |  Right Mouse Button  |  ESC\n"
                              "> Track Mouse        | > Confirm Signature  | > Discard Signature");

    /*
     * With open window
     */
    cout << "Launch ccsm-track application" << endl;
    while (window.isOpen())
    {


        Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case Event::Closed: {
                    path = "./signatures/";
                    path += args["name"].as<string>();
                    path += ".sgn.tsv";
                    vector<Track> tracks = trackPad.getStoredTracks();
                    saveTrackListToFile(tracks, path);
                    window.close();
                    break;
                }
                case Event::MouseButtonPressed: {
                    if (event.mouseButton.button == Mouse::Left) {
                        trackPad.trackMouseMovement();
                    } else if (event.mouseButton.button == Mouse::Button::Right) {
                        trackPad.storeTrack();
                    }
                    break;
                }
                case Event::MouseButtonReleased: {
                    //track Pad
                    if(event.mouseButton.button == Mouse::Left) {
                        trackPad.stopTracking();
                    }
                    break;
                }
                case Event::KeyPressed: {
                    if (event.key.code == Keyboard::Escape) {
                        trackPad.discardTrack();
                    }
                    break;
                }
                default:
                    break;
            }

            if (trackPad.isTrackingMouse()) {
                trackPad.trackMouseMovement();
            }
        }

    }

}
