#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

int main()
{
    //==================================================
    // Airfoil Parameters
    //==================================================

    int camber = 2;
    int position = 4;
    int thickness = 12;
    int reynolds = 500000;

    //==================================================
    // Polar file name
    //==================================================

    string polarFile =
    to_string(camber) +
    to_string(position);

if (thickness < 10)
    polarFile += "0";

polarFile += to_string(thickness);
polarFile += ".txt";

    //==================================================
    // Check if dataset.csv already exists
    //==================================================

    ifstream check("dataset.csv");
    bool fileExists = check.good();
    check.close();

    //==================================================
    // Open dataset.csv in append mode
    //==================================================

    ofstream csv("dataset.csv", ios::app);

    if (!csv)
    {
        cout << "Couldn't open dataset.csv\n";
        return 1;
    }

    // Write header only once
    if (!fileExists)
    {
        csv << "Camber,"
            << "Position,"
            << "Thickness,"
            << "Re,"
            << "Alpha,"
            << "CL,"
            << "CD,"
            << "CDp,"
            << "CM,"
            << "TopXtr,"
            << "BotXtr\n";
    }

    //==================================================
    // Open XFOIL polar file
    //==================================================

    ifstream file(polarFile);

    if (!file)
    {
        cout << "Couldn't open " << polarFile << endl;
        return 1;
    }

    string line;
    bool dataStarted = false;

    //==================================================
    // Parse the polar file
    //==================================================

    while (getline(file, line))
    {
        if (line.find("------") != string::npos)
        {
            dataStarted = true;
            continue;
        }

        if (dataStarted)
        {
            stringstream ss(line);

            double alpha;
            double cl;
            double cd;
            double cdp;
            double cm;
            double topXtr;
            double botXtr;

            ss >> alpha
               >> cl
               >> cd
               >> cdp
               >> cm
               >> topXtr
               >> botXtr;

            // Console output for debugging
            cout << alpha << " "
                 << cl << " "
                 << cd << endl;

            // Append one row to dataset.csv
            csv << camber << ","
                << position << ","
                << thickness << ","
                << reynolds << ","
                << alpha << ","
                << cl << ","
                << cd << ","
                << cdp << ","
                << cm << ","
                << topXtr << ","
                << botXtr << "\n";
        }
    }

    file.close();
    csv.close();

    cout << "\nDataset updated successfully!\n";

    return 0;
}
