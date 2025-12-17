#!/usr/bin/env python3
"""
Performance benchmark script to demonstrate optimization improvements.
This script compares the performance of the optimized KNN implementation.
"""

import time
import numpy as np
import cbr_knn

def benchmark_predictions(num_tests=100):
    """Benchmark the prediction performance"""
    print("=" * 60)
    print("KNN Performance Benchmark")
    print("=" * 60)
    
    # Initialize model
    print("\nInitializing model...")
    start = time.time()
    cbr_knn.init()
    init_time = time.time() - start
    print(f"Initialization time: {init_time:.4f} seconds")
    
    # Create random test samples
    print(f"\nRunning {num_tests} predictions...")
    test_samples = []
    for _ in range(num_tests):
        # Generate random test data similar to training data
        test_sample = np.array([
            np.random.randint(0, 17),      # Pregnancies
            np.random.randint(0, 200),     # Glucose
            np.random.randint(0, 122),     # Blood Pressure
            np.random.randint(0, 100),     # Skin Thickness
            np.random.randint(0, 846),     # Insulin
            np.random.uniform(0, 67.1),    # BMI
            np.random.uniform(0, 2.42),    # Diabetes Pedigree Function
            np.random.randint(21, 81)      # Age
        ])
        test_samples.append(test_sample)
    
    # Benchmark predictions
    start = time.time()
    for test_sample in test_samples:
        pred, indices, similarities, _ = cbr_knn.model.predict(test_sample)
    total_time = time.time() - start
    
    avg_time = total_time / num_tests
    predictions_per_sec = num_tests / total_time
    
    print(f"Total time: {total_time:.4f} seconds")
    print(f"Average time per prediction: {avg_time:.4f} seconds")
    print(f"Predictions per second: {predictions_per_sec:.2f}")
    
    print("\n" + "=" * 60)
    print("Optimization Summary:")
    print("=" * 60)
    print("✓ Vectorized similarity calculations (eliminates Python loops)")
    print("✓ NumPy argpartition for k-nearest neighbors (O(n) vs O(n log n))")
    print("✓ Pre-allocated byte arrays in protocol serialization")
    print("✓ External CSV data loading (faster module import)")
    print("✓ Removed redundant array copies in similarity calculation")
    print("=" * 60)

if __name__ == "__main__":
    benchmark_predictions()
