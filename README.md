
# Diabetes Diagnosis with Case Based Reasoning using K-Nearest Neighbor

## Introduction

Diabetes is a chronic disease that can potentially lead to serious complications if not managed properly. Technologies such as Case Based Reasoning (CBR) have been employed to assist in the diagnosis of this disease. This project applies the K-Nearest Neighbor (KNN) algorithm to a dataset with 768 samples, where 500 are non-diabetic and 268 are diabetic cases. By implementing KNN across three different testing scenarios, we evaluate the model's performance and consistency.

## Dataset

The dataset consists of 768 samples, with the following distribution:
- 500 non-diabetic cases
- 268 diabetic cases
  
The dataset includes various medical features such as:
- Pregnancies
- Glucose
- Blood Pressure
- Skin Thickness
- Insulin
- BMI
- Diabetes Pedigree Function
- Age

## Methodology

### K-Nearest Neighbor Algorithm

The KNN algorithm is used to classify the data into diabetic and non-diabetic categories. The algorithm works by finding the K nearest neighbors to a given test sample and classifies the sample based on the majority class of its neighbors and the similarity values.

### Scenarios

Three different scenarios are tested with varying proportions of training and testing data:
1. **Scenario 1: 60% Train, 40% Test**
2. **Scenario 2: 70% Train, 30% Test**
3. **Scenario 3: 80% Train, 20% Test**

### Evaluation Metrics

The performance of the model is evaluated using the following metrics:
- **Confusion Matrix**
- **Accuracy**
- **Precision**
- **Recall**
- **F1-Score**

## Results

### Scenario 1: 60% Train, 40% Test

- **Confusion Matrix:**
  
  ![Confusion Matrix Scenario 1](https://github.com/arcorium/Diabetes-Diagnosis/assets/72118120/72c32de6-7e5e-4e6b-a228-05fc0a85e602)

- **Evaluation Score:**
  
  ```
              precision   recall   f1-score   support

       0.0       0.81      0.60      0.69       206
       1.0       0.47      0.72      0.57       102

  accuracy                           0.64       308
  macro avg       0.64     0.66      0.63       308
  weighted avg    0.70     0.64      0.65       308
  ```

### Scenario 2: 70% Train, 30% Test

- **Confusion Matrix:**

  ![Confusion Matrix Scenario 2](https://github.com/arcorium/Diabetes-Diagnosis/assets/72118120/1af00d04-6c7e-42c9-aa29-e053a5954765)

- **Evaluation Score:**

  ```
                precision  recall   f1-score   support
  
        0.0       0.79      0.60      0.68       151
        1.0       0.48      0.70      0.57       80
  
  accuracy                            0.64       231
  macro avg       0.64      0.65      0.63       231
  weighted avg    0.68      0.64      0.65       231
  ```

### Scenario 3: 80% Train, 20% Test

- **Confusion Matrix:**
  
  ![Confusion Matrix Scenario 3](https://github.com/arcorium/Diabetes-Diagnosis/assets/72118120/5df9f067-db4e-48bd-af52-201199b4f1f6)

- **Evaluation Score:**
  
  ```
              precision   recall   f1-score    support
  
       0.0       0.79      0.56      0.65        99
       1.0       0.48      0.73      0.58        55
  
  accuracy                           0.62       154
  macro avg      0.63      0.64      0.61       154
  weighted avg   0.68      0.62      0.62       154
  ```

## Conclusion

The results indicate that the model's performance remains relatively consistent across different scenarios. Despite a slight decrease in accuracy in the scenario with 80% training data and 20% testing data, the uniformity in precision, recall, and F1-score demonstrates the reliability of the model in classifying diabetes cases. Thus, considering the variations in training and testing data proportions, it can be concluded that the CBR model performs well in identifying diabetes cases, with accuracy values ranging between 61.99% and 63.64%.
