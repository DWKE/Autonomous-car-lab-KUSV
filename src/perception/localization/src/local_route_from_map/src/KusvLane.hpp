/**
 * @copyright Konkuk University, Department of Smart Vehicle Engineering, 2018.
 * <br>All rights reserved. Subject to limited distribution and restricted
 * disclosure only.
 * @author kichun.jo@gmail.com
 * @file KusvLane.hpp
 * @brief Lane structure, import and export
 * @version 1.0
 * @date 2018-11-20
 * @bug	No known bugs
 * @warning	No warnings
 */

#pragma once
#include <ros/ros.h>
#include <string> // std::string
#include <vector> // std::vector

struct SKusvLanePoint {
  double m_dPtX_m;
  double m_dPtY_m;

public:
  SKusvLanePoint() : m_dPtX_m(0.0), m_dPtY_m(0.0) {}
  SKusvLanePoint(double dPtX_m, double dPtY_m)
      : m_dPtX_m(dPtX_m), m_dPtY_m(dPtY_m) {}
  SKusvLanePoint(const SKusvLanePoint &other)
      : m_dPtX_m(other.m_dPtX_m), m_dPtY_m(other.m_dPtY_m) {}
  ~SKusvLanePoint() {}
};

struct SKusvLane {
  unsigned int m_nLaneID;
  std::vector<SKusvLanePoint> m_vecKusvLanePoint;

public:
  SKusvLane() : m_nLaneID(0) { m_vecKusvLanePoint.clear(); }
  SKusvLane(unsigned int nLaneID, std::vector<SKusvLanePoint> vecKusvLanePoint)
      : m_nLaneID(nLaneID), m_vecKusvLanePoint(vecKusvLanePoint) {}
  SKusvLane(const SKusvLane &other)
      : m_nLaneID(other.m_nLaneID),
        m_vecKusvLanePoint(other.m_vecKusvLanePoint) {}
  ~SKusvLane() {}
};

struct SKusvLanes {
  std::vector<SKusvLane> m_vecKusvLanes;

public:
  SKusvLanes() { m_vecKusvLanes.clear(); }
  ~SKusvLanes() {}

public:
  bool ImportKusvLaneCsvFile(std::string strImportKusvLaneCsvFilePath);
  bool ExportKusvLaneCsvFile(std::string strExportKusvLaneCsvFilePath);

private:
  // Internal functions for file management
  const static int STRING_PARSING_MAX_SIZE = 1000;
  std::string TimeToString(void);
  size_t CountFileLines(std::istream &is);
  bool CheckPathExistence(std::string strFilePath);
  std::vector<std::string> FindFileList(std::string strPath,
                                        std::string strExtension);
  std::vector<std::string> SplitString(std::string strOrigin,
                                       std::string strTok);

  // Internal functions for debug information
  void DisplayInfo(std::string str);
  void DisplayError(std::string str);
  void DisplayProgress(std::string str, unsigned int nNumOfProgress,
                       unsigned int nNumOfTotal);
};
