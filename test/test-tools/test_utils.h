/**
 * Copyright 2020, Massachusetts Institute of Technology,
 * Cambridge, MA 02139
 * All Rights Reserved
 * Authors: Jingnan Shi, et al. (see THANKS for the full author list)
 * See LICENSE for the license information
 */

#pragma once

#include <vector>
#include <iostream>
#include <cmath>

#include <Eigen/Core>

#include "teaser/geometry.h"

namespace teaser {
namespace test {

inline std::vector<std::string> getNextLineAndSplitIntoTokens(std::istream& str) {
  std::vector<std::string> result;
  std::string line;
  std::getline(str, line);

  std::stringstream lineStream(line);
  std::string cell;

  while (std::getline(lineStream, cell, ',')) {
    result.push_back(cell);
  }
  // This checks for a trailing comma with no data after it.
  if (!lineStream && cell.empty()) {
    // If there was a trailing comma then add an empty element.
    result.push_back("");
  }
  return result;
}

/**
 * Read a (MATLAB generated) CSV file of a matrix into a Eigen matrix
 * @param objectFile
 * @return
 */
template <class T, int R, int C>
inline Eigen::Matrix<T, R, C> readFileToEigenMatrix(std::istream& objectFile) {
  Eigen::Matrix<T, R, C> object_in;
  size_t row = 0;
  // parsing csv in a row major order
  while (true) {
    auto tokens = teaser::test::getNextLineAndSplitIntoTokens(objectFile);
    if (tokens.size() <= 1) {
      break;
    }
    if (object_in.rows() <= row) {
      object_in.conservativeResize(row + 1, Eigen::NoChange);
    }
    if (object_in.cols() != tokens.size()) {
      object_in.conservativeResize(Eigen::NoChange, tokens.size());
    }
    for (size_t col = 0; col < tokens.size(); ++col) {
      object_in(row, col) = static_cast<T>(std::stod(tokens[col]));
    }
    row++;
  }
  return object_in;
}

template <class T, int R, int C>
inline Eigen::Matrix<T, R, C> readFileToEigenFixedMatrix(std::istream& objectFile) {
  Eigen::Matrix<T, R, C> object_in;
  size_t row = 0;
  // parsing csv in a row major order
  while (true) {
    auto tokens = teaser::test::getNextLineAndSplitIntoTokens(objectFile);
    if (tokens.size() < 1) {
      break;
    }
    if (row >= R) {
      break;
    }
    for (size_t col = 0; col < C; ++col) {
      object_in(row, col) = static_cast<T>(std::stod(tokens[col]));
    }
    row++;
  }
  return object_in;
}

inline double getAngularError(Eigen::Matrix3d R_exp, Eigen::Matrix3d R_est) {
  return std::abs(std::acos(fmin(fmax(((R_exp.transpose() * R_est).trace() - 1) / 2, -1.0), 1.0)));
}

template <class T>
inline Eigen::Matrix<T, 3, Eigen::Dynamic> teaserPointCloudToEigenMatrix(PointCloud cloud) {
  Eigen::Matrix<T,3,Eigen::Dynamic> mat(3, cloud.size());
  for (size_t i = 0; i < cloud.size(); ++i) {
    mat.col(i) << cloud[i].x, cloud[i].y, cloud[i].z;
  }
  return mat;
}

} // namespace test
}