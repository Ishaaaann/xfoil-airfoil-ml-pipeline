/*
==========================================================
XFOIL Airfoil ML Pipeline

Author: Ishaan Sharma

Description:
This program automates XFOIL to generate aerodynamic
datasets for NACA 4-digit airfoils.

Features:
- Automatic parameter sweeps
- Reynolds number sweep
- Angle of attack sweep
- Automatic polar generation
- CSV dataset generation
- Timeout protection
- Failed case logging

Output:
dataset.csv
failed_cases.csv
polar_output/

==========================================================
*/


#include <iostream>
#include <fstream>
#include <cstdlib>
#include <filesystem>
#include <vector>
#include <iomanip>
#include <sstream>
#include <windows.h>

using namespace std;
namespace fs = std::filesystem;

string getPolarFileName(
    int camber,
    int position,
    int thickness,
    int reynolds)
{
    string polarFile;

    polarFile += to_string(camber);
    polarFile += to_string(position);

    if (thickness < 10)
        polarFile += "0";

    polarFile += to_string(thickness);

    polarFile += "_Re";
    polarFile += to_string(reynolds);

    polarFile += ".txt";

    return "polar_output/" + polarFile;;
}

bool changeDirectory(){
    //==================================================
    // PROJECT FOLDER
    //==================================================

    string projectFolder = "C:\\Users\\ishaa\\OneDrive\\Desktop\\Ishaan\\projects\\Custom";

    //==================================================
    // CHANGE TO PROJECT DIRECTORY
    //==================================================

    try
    {
        fs::current_path(projectFolder);
    }
    catch (...)
    {
        cout << "Couldn't change directory.\n";
        return false;
    }

    cout << "Current Directory:\n";
    cout << fs::current_path() << "\n\n";
    return true;


}


void generate_script(
    int camber,
    int position,
    int thickness,
    int reynolds,
    int iterations,
    double aoaStart,
    double aoaEnd,
    double aoaIncrement){
 
    //==================================================
    // CREATE try.txt
    //==================================================

    ofstream file("try.txt");

    if (!file)
    {
        cout << "Couldn't create try.txt\n";
        return ;
    }

    file << "NACA ";

    file << camber;
    file << position;

    if (thickness < 10)
        file << "0";

    file << thickness << "\n";

    file << "OPER\n";

    file << "VISC " << reynolds << "\n";

    file << "ITER\n";
    file << iterations << "\n";

  file << "PACC\n";
  file << getPolarFileName(
            camber,
            position,
            thickness,
            reynolds)
     << "\n\n";

    file << "ASEQ\n";

    file << aoaStart << "\n";
    file << aoaEnd << "\n";
    file << aoaIncrement << "\n\n";

    file << "QUIT\n";

    file.close();


}
bool Run_Xfoil(int timeout_ms = 120000)
{
    //==============================================
    // Open try.txt as stdin
    //==============================================

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    HANDLE hInput = CreateFileA(
        "try.txt",
        GENERIC_READ,
        FILE_SHARE_READ,
        &sa,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hInput == INVALID_HANDLE_VALUE)
    {
        cout << "Couldn't open try.txt\n";
        return false;
    }

    //==============================================
    // Redirect stdout to NUL
    //==============================================

    HANDLE hOutput = CreateFileA(
        "NUL",
        GENERIC_WRITE,
        FILE_SHARE_WRITE,
        &sa,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hOutput == INVALID_HANDLE_VALUE)
    {
        CloseHandle(hInput);
        cout << "Couldn't open NUL device\n";
        return false;
    }

    //==============================================
    // Startup info
    //==============================================

    STARTUPINFOA si = {};
    si.cb = sizeof(si);

    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = hInput;
    si.hStdOutput = hOutput;
    si.hStdError = hOutput;

    PROCESS_INFORMATION pi = {};

    //==============================================
    // Launch XFOIL
    //==============================================

    char command[] = "xfoil.exe";

    BOOL success = CreateProcessA(
        NULL,
        command,
        NULL,
        NULL,
        TRUE,
        CREATE_NO_WINDOW,
        NULL,
        NULL,
        &si,
        &pi);

    CloseHandle(hInput);
    CloseHandle(hOutput);

    if (!success)
    {
        cout << "Couldn't start XFOIL.\n";
        return false;
    }

    //==============================================
    // Wait for completion
    //==============================================

    DWORD result =
        WaitForSingleObject(
            pi.hProcess,
            timeout_ms);

    if (result == WAIT_TIMEOUT)
    {
        cout << "XFOIL timed out.\n";

        TerminateProcess(pi.hProcess, 1);

        WaitForSingleObject(pi.hProcess, INFINITE);

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        return false;
    }

    DWORD exitCode;

    GetExitCodeProcess(
        pi.hProcess,
        &exitCode);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    if (exitCode != 0)
    {
        cout << "XFOIL exited with code "
             << exitCode
             << endl;

        return false;
    }

    return true;
}

void parser(
    int camber,
    int position,
    int thickness,
    int reynolds){
  
    //==================================================
    // Polar file name
    //==================================================

   string polarFile =
    getPolarFileName(
        camber,
        position,
        thickness,
        reynolds);

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
        return ;
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
        return ;
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

           if (!(ss >> alpha
         >> cl
         >> cd
         >> cdp
         >> cm
         >> topXtr
         >> botXtr))
{
    continue;
}

            // // Console output for debugging
            // cout << alpha << " "
            //      << cl << " "
            //      << cd << endl;

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


    return;
}

void logFailure(
    int camber,
    int position,
    int thickness,
    int reynolds)
{
    ifstream check("failed_cases.csv");
    bool exists = check.good();
    check.close();

    ofstream file("failed_cases.csv", ios::app);

    if (!exists)
    {
        file << "Camber,Position,Thickness,Re\n";
    }

    file << camber << ","
         << position << ","
         << thickness << ","
         << reynolds << "\n";

    file.close();
}
int main()
{
    int iterations = 200;

    double aoaStart = -5.0;
    double aoaEnd = 15.0;
    double aoaIncrement = 0.5;

    if (!changeDirectory())
        return 1;

    vector<int> reynoldsNumbers =
    {
        50000,
        100000,
        150000,
        200000,
        250000,
        300000,
        350000,
        400000,
        450000,
        500000,
        750000,
        1000000,
        1250000,
        1500000,
        1750000,
        2000000
    };

    const int totalSimulations =
        9 *          // camber (0-8)
        8 *          // position (1-8)
        9 *          // thickness (8-24 step 2)
        reynoldsNumbers.size();

    int simulation = 1;

    for (int camber = 0; camber <= 8; camber++)
    {
        for (int position = 1; position <= 8; position++)
        {
            for (int thickness = 8; thickness <= 24; thickness += 2)
            {
                for (int reynolds : reynoldsNumbers)
                {
                    generate_script(
                        camber,
                        position,
                        thickness,
                        reynolds,
                        iterations,
                        aoaStart,
                        aoaEnd,
                        aoaIncrement);

                    bool success = Run_Xfoil();

if (success)
{
    parser(
        camber,
        position,
        thickness,
        reynolds);
}
else
{
    logFailure(
        camber,
        position,
        thickness,
        reynolds);
}

                    double progress =
                        100.0 * simulation / totalSimulations;

                    cout << fixed << setprecision(1);

                    cout << "["
                         << simulation
                         << "/"
                         << totalSimulations
                         << "] ";

                    cout << progress << "%   ";

                    cout << "NACA ";

                    cout << camber;
                    cout << position;

                    if (thickness < 10)
                        cout << "0";

                    cout << thickness;

                    cout << "   Re=" << reynolds;

                    if (!success)
                        cout << "   (FAILED)";

                    cout << "\n";

                    simulation++;
                }
            }
        }
    }

    cout << "\n=====================================\n";
    cout << "Dataset generation complete.\n";
    cout << "Total simulations: " << totalSimulations << endl;
    cout << "=====================================\n";

    return 0;
}
