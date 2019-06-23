/**
 * @copyright Konkuk University, Department of Smart Vehicle Engineering, 2018.
 * <br>All rights reserved. Subject to limited distribution and restricted
 * disclosure only.
 * @author kichun.jo@gmail.com
 * @file KusvLane.cpp
 * @brief Lane structure, import and export
 * @version 1.0
 * @date 2018-11-20
 * @bug	No known bugs
 * @warning	No warnings
 */

#include "KusvLane.hpp"

#include <boost/filesystem.hpp> // boost::filesystem
#include <ctime>
#include <fstream>  // std::ifstream
#include <iomanip>  // std::setw
#include <iostream> // std::cout
#include <sstream>
#include <stdio.h> // remove

bool SKusvLanes::ImportKusvLaneCsvFile(
    std::string strImportKusvLaneCsvFilePath) {
  DisplayInfo("Import Name is " + strImportKusvLaneCsvFilePath);
  std::vector<std::string> vecStrLaneCsvFiles =
      FindFileList(strImportKusvLaneCsvFilePath, ".csv");

  // ROS_INFO("%s", strImportKusvLaneCsvFilePath.c_str());
  // ROS_INFO("%d", vecStrLaneCsvFiles.size());

  if (vecStrLaneCsvFiles.size() <= 0) {
    DisplayError("No availabe Kusv Lane Csv files");
    return false;
  }

  // Clear the Lanes vector and Id
  m_vecKusvLanes.clear();
  int nLaneID = 0;

  for (auto vecIt = vecStrLaneCsvFiles.begin();
       vecIt != vecStrLaneCsvFiles.end(); vecIt++) {
    // Lane file path
    std::string strLaneFile =
        strImportKusvLaneCsvFilePath + "/" + *vecIt + ".csv";
    ROS_INFO("%s", strLaneFile.c_str());

    // One Lane structure and ID
    SKusvLane tmpKusvLane;

    std::vector<std::string> vecstrSplitLine = SplitString(*vecIt, "_");
    tmpKusvLane.m_nLaneID = (unsigned int)(stoi(vecstrSplitLine[1]));

    // File read
    std::ifstream ifsFile(strLaneFile);
    unsigned int nTotalNumIter =
        (unsigned int)CountFileLines(ifsFile); // Get number of lines
    bool bFlagFirstLine = true;
    unsigned int nNumIter = 0;

    while (!ifsFile.eof()) {

      // Progress display
      DisplayProgress("Parse Kusv Lane Csv File", nNumIter, nTotalNumIter - 1);
      nNumIter++;

      // Get line information
      char inputString[STRING_PARSING_MAX_SIZE];
      ifsFile.getline(inputString, STRING_PARSING_MAX_SIZE);

      // First line check
      if (bFlagFirstLine == true) {
        bFlagFirstLine = false;
        continue;
      }

      // Empty line check
      std::string strTmpLine = std::string(inputString);
      if (strTmpLine.compare("") == 0)
        continue;

      // Get data
      std::vector<std::string> vecstrSplitLine = SplitString(strTmpLine, ",");

      // Parse
      tmpKusvLane.m_vecKusvLanePoint.push_back(SKusvLanePoint(
          (double)stod(vecstrSplitLine[0]), (double)stod(vecstrSplitLine[1])));
    }
    // Push back to m_vecKusvLanes
    m_vecKusvLanes.push_back(tmpKusvLane);

    // Close file
    ifsFile.close();
  }

  // return
  return true;
}

bool SKusvLanes::ExportKusvLaneCsvFile(
    std::string strExportKusvLaneCsvFilePath) {
  // Generate path
  if (CheckPathExistence(strExportKusvLaneCsvFilePath) != true) {
    DisplayError("ExportKusvLaneCsvFile Failed - File path cannot find!");
    return false;
  }

  for (auto vecLanesIt = m_vecKusvLanes.begin();
       vecLanesIt != m_vecKusvLanes.end(); vecLanesIt++) {
    SKusvLane &tmpKusvLane = *vecLanesIt;

    // Configuration
    std::string strRecordingFile =
        strExportKusvLaneCsvFilePath + "/" + "KusvLane_" +
        std::to_string(tmpKusvLane.m_nLaneID) + ".csv";
    std::ofstream file_out;
    file_out.open(strRecordingFile.c_str());
    file_out.precision(12);

    // File header
    file_out << "LanePointX,"
             << "LanePointY"
             << "\n";

    for (auto vecLaneIt = tmpKusvLane.m_vecKusvLanePoint.begin();
         vecLaneIt != tmpKusvLane.m_vecKusvLanePoint.end(); vecLaneIt++) {
      SKusvLanePoint &tmpKusvLanePoint = *vecLaneIt;

      file_out << std::to_string(tmpKusvLanePoint.m_dPtX_m) << ","
               << std::to_string(tmpKusvLanePoint.m_dPtY_m) << "\n";
    }

    // Export
    file_out.close();
    DisplayInfo("Export complete: " + strRecordingFile);
  }
  return true;
}

std::string SKusvLanes::TimeToString(void) {
  struct tm *t;
  time_t timer;

  timer = time(NULL);
  t = localtime(&timer);

  std::stringstream ssTmp;
  ssTmp << std::setw(2) << std::setfill('0') << t->tm_year + 1900;
  ssTmp << std::setw(2) << std::setfill('0') << t->tm_mon + 1;
  ssTmp << std::setw(2) << std::setfill('0') << t->tm_mday;
  ssTmp << "_";
  ssTmp << std::setw(2) << std::setfill('0') << t->tm_hour;
  ssTmp << std::setw(2) << std::setfill('0') << t->tm_min;
  ssTmp << std::setw(2) << std::setfill('0') << t->tm_sec;
  return ssTmp.str();
}

size_t SKusvLanes::CountFileLines(std::istream &is) {
  // skip when bad
  if (is.bad())
    return 0;
  // save state
  std::istream::iostate state_backup = is.rdstate();
  // clear state
  is.clear();
  std::istream::streampos pos_backup = is.tellg();

  is.seekg(0);
  size_t line_cnt;
  size_t lf_cnt = std::count(std::istreambuf_iterator<char>(is),
                             std::istreambuf_iterator<char>(), '\n');
  line_cnt = lf_cnt;
  // if the file is not end with '\n' , then line_cnt should plus 1
  is.unget();
  if (is.get() != '\n') {
    ++line_cnt;
  }

  // recover state
  is.clear(); // previous reading may set eofbit
  is.seekg(pos_backup);
  is.setstate(state_backup);

  return line_cnt;
}

bool SKusvLanes::CheckPathExistence(std::string strFilePath) {
  // Create a Path object from given path string
  boost::filesystem::path pathObj(strFilePath);
  // Check if path exists and is of a directory file
  if (boost::filesystem::exists(pathObj) &&
      boost::filesystem::is_directory(pathObj))
    return true;
  else {
    if (boost::filesystem::create_directories(strFilePath.c_str()) != true) {
      return false;
    }
  }

  return true;
}

std::vector<std::string> SKusvLanes::FindFileList(std::string strPath,
                                                  std::string strExtension) {
  // Define output vector
  std::vector<std::string> vecFileList;
  vecFileList.clear();

  if (!strPath.empty()) {
    // Set path
    boost::filesystem::path apk_path(strPath);

    // Set iterator
    boost::filesystem::recursive_directory_iterator end;

    // iteration
    for (boost::filesystem::recursive_directory_iterator i(apk_path); i != end;
         ++i) {
      const boost::filesystem::path cp = (*i);
      // Get file name with extension
      std::string strFileName = cp.filename().string();
      std::string strFileExt = boost::filesystem::extension(strFileName);

      // Check extension
      if (std::string(strFileExt).compare(strExtension) != 0)
        continue;

      // Get file name
      vecFileList.push_back(cp.stem().string());
    }
  }
  return vecFileList;
}

std::vector<std::string> SKusvLanes::SplitString(std::string strOrigin,
                                                 std::string strTok) {
  size_t cutAt;
  std::vector<std::string> vecstrSplitInputLine;
  while ((cutAt = strOrigin.find_first_of(strTok)) != strOrigin.npos) {
    if (cutAt > 0) {
      vecstrSplitInputLine.push_back(strOrigin.substr(0, cutAt));
    }

    strOrigin = strOrigin.substr(cutAt + 1);
  }

  if (strOrigin.length() > 0) {
    vecstrSplitInputLine.push_back(strOrigin.substr(0, cutAt));
  }
  return vecstrSplitInputLine;
}

void SKusvLanes::DisplayInfo(std::string str) { std::cout << str << std::endl; }
void SKusvLanes::DisplayError(std::string str) {
  std::cerr << str << std::endl;
}
void SKusvLanes::DisplayProgress(std::string str, unsigned int nNumOfProgress,
                                 unsigned int nNumOfTotal) {
  if (nNumOfTotal == 0) {
    DisplayError("Cannot display progress");
  }

  static int prev_progress = 0;
  int pres_progress =
      (int)((double)nNumOfProgress / (double)nNumOfTotal * 100.);

  if (prev_progress != pres_progress) {
    std::cout << str << ": ";
    prev_progress = pres_progress;
    float progress = (float)pres_progress / 100.f;
    int barWidth = 30;
    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
      if (i < pos)
        std::cout << "=";
      else if (i == pos)
        std::cout << ">";
      else
        std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
  }

  if (nNumOfProgress == nNumOfTotal) {
    std::cout << std::endl;
  }
}