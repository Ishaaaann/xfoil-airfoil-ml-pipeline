
# XFOIL Airfoil ML Pipeline

An end-to-end pipeline for generating aerodynamic datasets using XFOIL and training machine learning surrogate models for custom airfoil optimization.

---

## Overview

This project automates the generation of aerodynamic datasets for NACA 4-digit airfoils using XFOIL. The generated dataset is intended for training machine learning models that can rapidly predict aerodynamic coefficients, enabling future optimization using Genetic Algorithms.

Instead of running thousands of expensive XFOIL simulations during optimization, the trained neural network will act as a fast aerodynamic surrogate model.

---

## Features

- Automated XFOIL execution
- Automatic generation of NACA 4-digit airfoils
- Reynolds number parameter sweep
- Angle of attack sweep
- Automatic polar file generation
- Automatic parsing of XFOIL output
- Dataset generation in CSV format
- Timeout protection for stalled simulations
- Failed simulation logging
- Organized output directory structure

---

## Parameter Space

| Parameter | Range |
|-----------|-------|
| Maximum Camber | 0% – 8% (step 1%) |
| Camber Position | 10% – 80% chord (step 10%) |
| Maximum Thickness | 8% – 24% (step 2%) |
| Reynolds Number | 50,000 – 2,000,000 |
| Angle of Attack | -5° to 15° (step 0.5°) |

---

## Pipeline

```
Generate XFOIL Script
        │
        ▼
Run XFOIL
        │
        ▼
Generate Polar File
        │
        ▼
Parse Polar Data
        │
        ▼
Append to dataset.csv
        │
        ▼
Machine Learning Dataset
```

---

## Dataset Statistics

- Total simulations attempted: **10,368**
- Successful simulations: **10,201**
- Failed simulations: **167**
- Success rate: **98.4%**
- Total aerodynamic samples: **370,585**

The generated dataset contains:

- Camber
- Camber Position
- Thickness
- Reynolds Number
- Angle of Attack
- Lift Coefficient (CL)
- Drag Coefficient (CD)
- Pressure Drag (CDp)
- Pitching Moment Coefficient (CM)
- Transition Locations

---

## Repository Structure

```
.
├── src/
│   └── main.cpp
├── datasets/
│   ├── dataset.csv
│   └── failed_cases.csv
├── xfoil/
├── README.md
├── LICENSE
└── .gitignore
```

---

## Future Work

- Data preprocessing and visualization
- Neural network surrogate model
- Hyperparameter tuning
- Genetic Algorithm optimization
- Support for CST and Selig airfoil parameterizations
- Interactive airfoil design tool

---

## Technologies Used

- C++
- XFOIL
- Windows API
- CSV Data Processing

Future additions:

- Python
- TensorFlow / PyTorch
- Scikit-Learn
- Genetic Algorithms

---

## Author

**Ishaan Sharma**

Computer Engineering Undergraduate | Motorsport Aerodynamics | Machine Learning | CFD

---

## License

This project is licensed under the MIT License.
