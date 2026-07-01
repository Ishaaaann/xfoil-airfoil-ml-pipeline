# XFOIL Airfoil ML Pipeline

An end-to-end machine learning pipeline for aerodynamic analysis and optimization of NACA 4-digit airfoils using **XFOIL**, **Python**, **C++**, and **Neural Networks**.

The objective is to replace expensive XFOIL evaluations with a fast surrogate model capable of predicting aerodynamic coefficients, which will later be integrated into a Genetic Algorithm for airfoil optimization.

---

## Project Pipeline

```
NACA Airfoil Generation
          │
          ▼
   XFOIL Automation (C++)
          │
          ▼
 Dataset Generation
          │
          ▼
Exploratory Data Analysis
          │
          ▼
 Data Preprocessing
          │
          ▼
 Neural Network Surrogate
          │
          ▼
 Genetic Algorithm
          │
          ▼
 Optimized Airfoils
```

---

# Current Progress

- ✅ XFOIL automation completed
- ✅ Automatic dataset generation completed
- ✅ Exploratory Data Analysis completed
- ⬜ Data preprocessing
- ⬜ Neural network training
- ⬜ Hyperparameter optimization
- ⬜ Genetic algorithm optimization

---

# Repository Structure

```
xfoil-airfoil-ml-pipeline/

│
├── datasets/
│   ├── raw_dataset.csv
│   └── cleaned_dataset.csv
│
├── notebooks/
│   ├── EDA.ipynb
│   ├── preprocessing.ipynb
│   ├── training.ipynb
│   └── optimization.ipynb
│
├── polar_output/
│
├── src/
│
├── images/
│
├── README.md
├── LICENSE
└── .gitignore
```

---

# Dataset

### Airfoil Parameters

| Parameter | Range |
|-----------|-------|
| Camber | 0–8% |
| Camber Position | 10–80% chord |
| Thickness | 8–24% |
| Reynolds Number | 50,000 – 2,000,000 |
| Angle of Attack | −5° to 25° |

---

### Output Variables

- Lift Coefficient (CL)
- Drag Coefficient (CD)
- Pressure Drag (CDp)
- Pitching Moment (CM)
- Upper Surface Transition Location
- Lower Surface Transition Location

---

### Dataset Statistics

| Metric | Value |
|--------|-------|
| Total Simulations | 15,552 |
| Successful Simulations | 15,385 |
| Final Dataset Size | 416,234 unique operating points |
| Features | 11 |
| Missing Values | 0 |
| Duplicate Rows | 0 (after cleaning) |

---

# Exploratory Data Analysis

The generated dataset was validated before model development through an extensive exploratory data analysis.

The EDA includes:

- Data quality assessment
- Missing value analysis
- Duplicate removal
- Aerodynamic validation
- Distribution analysis
- Correlation analysis
- Reynolds number effects
- Stall behaviour verification
- Outlier inspection

Notebook:

```
notebooks/EDA.ipynb
```

---

# Reynolds Number Effects

One of the most important validations performed during the EDA was examining the effect of Reynolds number on the aerodynamic characteristics averaged across all generated airfoils.

<img width="1174" height="417" alt="Screenshot 2026-07-01 150633" src="https://github.com/user-attachments/assets/56612d4e-429b-42e8-9939-8f4fb890868c" />


**Observed trends**

- Higher Reynolds numbers produce higher lift coefficients.
- Stall is delayed as Reynolds number increases.
- Drag decreases significantly with increasing Reynolds number.
- Post-stall behaviour is successfully captured in the dataset.
- These trends closely match expected aerodynamic behaviour predicted by XFOIL.

---

# Technologies Used

- C++
- Python
- XFOIL 6.99
- Pandas
- NumPy
- Matplotlib
- Jupyter Notebook
- Git
- GitHub

---

# Future Work

- Feature engineering
- Data normalization
- Neural network surrogate model
- Hyperparameter tuning
- Model validation
- Genetic algorithm optimization
- Airfoil shape optimization
- Comparison against XFOIL predictions

---

# Motivation

Traditional aerodynamic optimization requires thousands of expensive XFOIL evaluations.

This project aims to develop a machine learning surrogate capable of predicting aerodynamic coefficients in milliseconds while maintaining high accuracy, enabling rapid design-space exploration and optimization.

---

# Author

**Ishaan Sharma**

Computer Engineering Undergraduate

Motorsports Aerodynamics • Machine Learning • CFD • Formula Student
