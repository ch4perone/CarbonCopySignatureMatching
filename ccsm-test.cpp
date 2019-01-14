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
    TrainingData trainingData(net.getTopology()[0] / 2);
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
                        vector<unsigned> train = {615, 293, 615, 295, 615, 302, 615, 316, 615, 332, 611, 349, 609, 367, 605, 375, 594, 404, 580, 432, 560, 461, 538, 486, 510, 503, 481, 526, 458, 540, 441, 551, 416, 560, 389, 565, 360, 565, 344, 565, 335, 565, 328, 565, 323, 565, 319, 563, 317, 560, 316, 557, 311, 549, 309, 535, 311, 516, 313, 492, 329, 459, 343, 447, 355, 443, 359, 438, 371, 436, 388, 436, 409, 443, 432, 458, 464, 494, 507, 536, 551, 584, 596, 644, 649, 704, 694, 745, 739, 779, 772, 809, 803, 828, 820, 839, 845, 845, 847, 842, 848, 838, 850, 837, 852, 832, 852, 830, 852, 827, 852, 822};

                        /*
                         * TODO CLEAN ALL THIS UP >> move to TrackPad::validateTrack(Net ...)
                         */
                        Track track = trackPad.getCurrentTrack();
                        vector<pair<int, int>> trackVector;
                        for(Vector2f v : track) {
                            trackVector.push_back({static_cast<const int &>(v.x), static_cast<const int &>(v.y)});
                        }
                        /*for (int i = 0; i < train.size(); i+=2) {
                            trackVector.push_back({train[i], train[i+1]});
                        }*/
                        DataPiece dp = trainingData.generateDataPiece(trackVector);
                        net.feedForward(dp.X);
                        vector<double> prediction;
                        net.getOutput(prediction);
                        string s = "Prediction: ";
                        for (double p : prediction) {
                            s+=to_string(p) + " ";
                        }
                        trackPad.drawInfoText(s);
                        trackPad.clearTrack();
                        window.display();
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
