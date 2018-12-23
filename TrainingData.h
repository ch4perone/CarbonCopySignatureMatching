#ifndef CARBONCOPYSIGNATUREMATCHING_TRAININGDATA_H
#define CARBONCOPYSIGNATUREMATCHING_TRAININGDATA_H

#include <string>
#include <vector>

using namespace std;

typedef vector<vector<double> > matrix;

class TrainingData
{
public:
    TrainingData(vector<string> filename_v);
    ~TrainingData() {/*for(ifstream *i : m_trainingDataFiles){
            delete(i);
        }
        m_trainingDataFiles.clear();
    */};
    //bool isEOF() { return m_trainingDataFile.eof();}
    void getTopology(vector<unsigned> &topology);

    unsigned getNextInputs(vector<double> &inputVals);
    unsigned getTargetOutputs(vector<double> &targetOutputVals);
    vector<string> m_trainingDataPaths;
    int c_inputNum;
    int total_inputNum;
    //vector<ifstream*> m_trainingDataFiles;
    vector<matrix> input_cube;
private:

};

#endif //CARBONCOPYSIGNATUREMATCHING_TRAININGDATA_H
