# Created By Muhammad Syaugi Shahab
# https://github.com/syaugis

import numpy as np
from collections import Counter


class KNN:
  # Class constants for similarity calculation
  WEIGHTS = np.array([0.7, 0.9, 0.5, 0.8, 0.9, 0.6, 0.5, 0.9])
  THRESHOLDS = np.array([4, 121, 69, 21, 80, 32, 0.47, 30])
  
  def __init__(self, k):
    self.k = k

  def fit(self, X_train, y_train):
    self.X_train = X_train
    self.y_train = y_train

  def predict(self, X_test):
    # Vectorized similarity calculation for all training samples at once
    similarities = self.calculate_similarity_vectorized(X_test, self.X_train)
    
    # Get indices of k largest similarities using argpartition (more efficient than full sort)
    # argpartition is O(n) vs O(n log n) for sort
    if len(similarities) > self.k:
      k_indices = np.argpartition(similarities, -self.k)[-self.k:]
      # Sort only the k elements
      k_indices = k_indices[np.argsort(similarities[k_indices])[::-1]]
    else:
      k_indices = np.argsort(similarities)[::-1]
    
    neighbors_indices = k_indices.tolist()
    neighbors_similarities = similarities[k_indices].tolist()
    neighbors = self.y_train[k_indices]
    
    # Prediksi dengan mayoritas kelas tetangga
    pred = Counter(neighbors).most_common(1)[0][0]
    
    # Create full similarities list for compatibility
    all_indices = np.argsort(similarities)[::-1]
    similarities_list = [(similarities[i], int(i)) for i in all_indices]
    
    return pred, neighbors_indices, neighbors_similarities, similarities_list

  def calculate_similarity(self, x1, x2):
    # Vectorized threshold comparison - much faster than individual if statements
    x1_test = (x1 > self.THRESHOLDS).astype(int)
    x2_train = (x2 > self.THRESHOLDS).astype(int)

    # Kemiripan (s) dihitung sebagai 1 jika gejala terdapat pada kedua kasus, 0 jika tidak
    similarities = (x1_test == x2_train).astype(int)

    # Perhitungan similarity dengan formula: (s1*w1 + s2*w2 + ... + sn*wn) / (w1 + w2 + ... + wn)
    weighted_similarity = np.sum(similarities * self.WEIGHTS) / np.sum(self.WEIGHTS)
    return weighted_similarity
  
  def calculate_similarity_vectorized(self, x_test, X_train):
    """Vectorized version of calculate_similarity for all training samples at once"""
    # Broadcast comparison: x_test is (8,), X_train is (n, 8)
    # Result will be (n, 8) boolean array
    x_test_binary = (x_test > self.THRESHOLDS).astype(int)
    X_train_binary = (X_train > self.THRESHOLDS).astype(int)
    
    # Compare test sample with all training samples at once
    # similarities shape: (n, 8)
    similarities = (x_test_binary == X_train_binary).astype(int)
    
    # Calculate weighted similarity for all samples: (n, 8) * (8,) -> (n, 8) -> (n,)
    weighted_similarities = np.sum(similarities * self.WEIGHTS, axis=1) / np.sum(self.WEIGHTS)
    
    return weighted_similarities


def read_data(file_path):
  data = np.genfromtxt(file_path, delimiter=',', skip_header=1)
  X = data[:, :-1]  # Fitur
  y = data[:, -1]  # Label
  return X, y


def add_new_data(data, new_data, add=True):
  if add:
    data = np.vstack([data, new_data])
  return data


model = KNN(3)
# Data is now loaded from CSV file in init() for better performance
data = None


def init():
  global data
  import os
  
  # Load data from CSV file (much faster module loading)
  csv_path = os.path.join(os.path.dirname(__file__), 'diabetes_train_data.csv')
  data_array = np.genfromtxt(csv_path, delimiter=',', skip_header=1)
  
  # Store for protocol.py access
  data = data_array.tolist()
  
  X_train = data_array[:, :-1]  # Fitur
  y_train = data_array[:, -1]  # Label
  np.set_printoptions(formatter={'float': '{: 0.3f}'.format})

  # Inisialisasi dan pelatihan model
  model.fit(X_train, y_train)
