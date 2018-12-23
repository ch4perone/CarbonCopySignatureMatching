#include <iostream>
#include <cxxopts.hpp>

/*
 * This is CCSM in its full glory
 *
 * compile: g++ --std=c++17 -o ccsm-train ccsm-train.cpp
 */

using namespace std;

cxxopts::ParseResult parseArguments(int argc, char **argv) {
    cxxopts::Options options("ccsm-train", "Carbon Copy Signature Matching - training application.\nTrain Neural Network(s) on digital signature tracks for classification");
    try {
        options.add_options()
                ("h,help", "Print help")
                ("d,debug", "Enable debugging");
        options.add_options("Data")
                ("t,trainingData", "File name", cxxopts::value<string>())
                ("o,out","Network name", cxxopts::value<string>())
                ("augment","Use in-build data augmentation (marginal data point shifts)");
        options.add_options("Training")
                ("trainTestSplit", "Proportion of training data [default: 0.8]", cxxopts::value<float>())
                ("epochs","Number of epochs (if not stopped early)", cxxopts::value<int>())
                ("earlyStopping", "Activate early-stopping", cxxopts::value<bool>())
                ("dropout", "Drop out proportion of neurons for training [default: 0.0]", cxxopts::value<float>())
                ("batchNormalization","Batches will probably not be implemented soon :D");
        options.add_options("Network")
                ("resolution","Define data resolution (data points used as network input), equivalent to input layer width of the network (plus an additional bias-neuron)", cxxopts::value<int>())
                ("depth", "Number of dense layer between input and output layers", cxxopts::value<int>())
                ("width", "Number of neurons per dense layer (including bias-neurons)", cxxopts::value<int>());
        options.add_options("Bagging (bootstrapping and aggregation)")
                ("m,multiples","Number of network copies, individually trained using multi-threading", cxxopts::value<int>())
                ("b,bootstrapping","Use bootstrapping to increase robustness of replicas")
                ("bootstrappingVariance", "Proportional variance in withheld input data and network composition [default: 0.2]", cxxopts::value<float>());


        auto result = options.parse(argc, argv);


        if (result.count("help")) {
            cout << options.help({"", "Group", "Data", "Training", "Network", "Replication"}) << endl;
            exit(0);
        }
        return result;

    } catch (const cxxopts::OptionException& e)
    {
        cout << "error parsing options: " << e.what() << endl;
        exit(1);
    }

}



int main(int argc, char** argv) {

    //Parse input arguments
    auto args = parseArguments(argc, argv);
    auto argsList = args.arguments();



    // Train network(s)
    if (!args.count("multiples")) {
        //Generate a single initial neural network

        //Train network
    }
    else {
        cout << "producing " << args["multiples"].as<int>() << " networks" << endl;
        //Generate and train multiple neural networks
        for (int i = 0; i < args["multiples"].as<int>(); ++i) {
            cout << args["o"].as<string>() << i << endl;
        }

    }


    //cout << argsList.at(0).key() << " " << argsList.at(0).value() << endl;

    //Train network(s)



    //




    cout << "Ciao, World!" << endl;
    return 0;
}