
#include <iostream>
#include <vector>

using namespace std;

void computeHistogram(vector<double> &vec_values);
double findMax(vector<double> &vec_values);
double findMin(vector<double> &vec_values);
double findAve(vector<double> &vec_values);

int main(int argc, const char * argv[]) {

    vector<double> arrValues;
    //Add values to vector array.
    arrValues.push_back(2334);
    arrValues.push_back(3343);
    arrValues.push_back(42364);
    arrValues.push_back(336);
    arrValues.push_back(333);
    arrValues.push_back(237);
    computeHistogram(arrValues);
    return 0;
}


void computeHistogram(vector<double> &vec_values)
{
    double max = findMax(vec_values);
    double average = findAve(vec_values);
    int hist_length = int (max/average);
    vector<int> hist_values(hist_length);
    cout<<"Average is : "<<average<<"\n";
    cout<<"Histogram length is : "<<hist_length<<"\n";
    for(unsigned int i=0; i<vec_values.size(); i++)
    {
        if(vec_values.at(i)>average*hist_length)
        {
            hist_values.at(hist_length-1)+=1;
        }
        for(int j=0; j<hist_length; j++)
        {
            if(vec_values.at(i)<(average*(j+1)))
            {
                hist_values.at(j)+=1;
                break;
            }
        }
    }
    for(unsigned int k=0; k<hist_values.size(); k++)
    {
        cout<<"Number of values at index "<<k<<" is "<<hist_values.at(k)<<" \n";
    }
}


double findMax(vector<double> &vec_values)
{
    double max = vec_values.at(0);
    for (unsigned int i=0; i<vec_values.size(); i++)
    {
        if(vec_values.at(i)>max)
        {
            max = vec_values.at(i);
        }
    }
    return max;
}


double findMin(vector<double> &vec_values)
{
    double min = vec_values.at(0);
    for (unsigned int i=0; i<vec_values.size(); i++)
    {
        if(vec_values.at(i)<min)
        {
            min = vec_values.at(i);
        }
    }
    return min;
}

double findAve(vector<double> &vec_values)
{
    double average,sum=0;
    for (unsigned int i=0; i<vec_values.size(); i++)
    {
        sum = sum+vec_values.at(i);
    }
    average = sum/vec_values.size();
    return average;
}
