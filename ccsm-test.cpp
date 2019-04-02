#include <iostream>
#include <cxxopts.hpp>
#include <SFML/Graphics.hpp>
#include <fstream>
#include "TrackPad.h"
#include "Net.h"
#include "TrainingData.h"

//compile: g++ -c ccsm-track.cpp
//then: g++ ccsm-track.o -o ccsm-track -lsfml-graphics -lsfml-window -lsfml-system

using namespace std;
using namespace sf;




cxxopts::ParseResult parseTestArguments(int argc, char **argv) {
    cxxopts::Options options("ccsm-track", "Carbon Copy Signature Matching - record a specific signature by tracking the mouse movement");
    try {
        options.add_options()
                ("h,help", "Print help")
                ("n,name", "Network name", cxxopts::value<string>());

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


int main(int argc, char** argv) {
    auto args = parseTestArguments(argc, argv);
    string path = "./signatures/";


    /*
     * Window setup
     */
    string window_name = "Test Signatures with network  " + args["name"].as<string>();

    RenderWindow window(VideoMode(1000, 1000), window_name);
    window.setFramerateLimit(120);
    TrackPad trackPad(window, "Left Mouse Button    "
                              "|  Right Mouse Button  |  ESC\n"
                              "> Track Mouse        | > Test Signature     | > Discard Signature");

    /*
     * Load neural network
     */
    Net net(args["name"].as<string>());
    int resolution = net.getTopology()[0] / 2;
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
                    window.close();
                    break;
                }
                case Event::MouseButtonPressed: {
                    if (event.mouseButton.button == Mouse::Left) {
                        trackPad.trackMouseMovement();
                    } else if (event.mouseButton.button == Mouse::Button::Right) {

                        Track track = trackPad.getCurrentTrack();
                        vector<pair<int, int>> trackVector;
                        for(Vector2f v : track) {
                            trackVector.push_back({static_cast<const int &>(v.x), static_cast<const int &>(v.y)});
                        }
                        /*for (int i = 0; i < train.size(); i+=2) {
                            trackVector.push_back({train[i], train[i+1]});
                        }*/
                        DataPiece dp = TrainingData::generateDataPiece(trackVector, resolution);
                        net.feedForward(dp.X);
                        vector<double> prediction;
                        net.getOutput(prediction);
                        string s = "Prediction: ";
                        for (double p : prediction) {
                            s+=to_string(p).substr(0,4) + " ";
                        }
                        trackPad.drawInfoText(s);
                        trackPad.drawCurrentTrack(false);
                        trackPad.clearTrack();
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
