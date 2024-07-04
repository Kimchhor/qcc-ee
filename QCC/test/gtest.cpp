// tests.cpp
#include <gtest/gtest.h>
#include <iostream>

#include "../lib/QCCMachineDescription.cpp"
using json = nlohmann::json;
using namespace std;

TEST(QCCMachineDescriptionTest, ConnectionMatrixCheck) {

  string path = "./machine_description.json";
  string jsonString = qcc::readFileIntoString(path);
  qcc::MachineDescription md = json::parse(jsonString);

  qcc::QCCMachineDescription QMD(8, md);

  vector<vector<int>> adjMatrix = QMD.getConnectionAdjMatrix();

  ASSERT_EQ(adjMatrix[0][7], 1);
  ASSERT_EQ(adjMatrix[7][2], 1);
  ASSERT_EQ(adjMatrix[12][7], 1);
  ASSERT_EQ(adjMatrix[12][19], 1);
  ASSERT_EQ(adjMatrix[19][14], 1);
  ASSERT_EQ(adjMatrix[14][7], 1);
  ASSERT_EQ(adjMatrix[2][7], 1);
  ASSERT_EQ(adjMatrix[9][14], 1);
  ASSERT_EQ(adjMatrix[9][4], 0);
  ASSERT_EQ(adjMatrix[9][16], 0);
  ASSERT_EQ(adjMatrix[21][16], 0);
  ASSERT_EQ(adjMatrix[21][26], 0);
  ASSERT_EQ(adjMatrix[19][26], 0);
  ASSERT_EQ(adjMatrix[19][24], 0);
}

void NeighborConnectionTest(vector<int> arr, vector<int> neighbors, int index ){

  for (int i: arr){
    string message = "ID " + std::to_string(i) + " is should be neigbor ID " + std::to_string(index);
      ASSERT_TRUE(std::find(neighbors.begin(),
                            neighbors.end(), i)!= neighbors.end()) << message;
  }                    
}

TEST(QCCMachineDescriptionTest, NeighborConnectionCheck) {

  string path = "./machine_description.json";
  string jsonString = qcc::readFileIntoString(path);
  qcc::MachineDescription md = json::parse(jsonString);

  qcc::QCCMachineDescription QMD(8, md);

  vector<vector<int>> adjMatrix = QMD.getConnectionAdjMatrix();

  ASSERT_EQ(QMD.getDiagonalNeighborNodes(0).size(), 1);
  ASSERT_EQ(QMD.getDiagonalNeighborNodes(2).size(), 2);
  ASSERT_EQ(QMD.getDiagonalNeighborNodes(9).size(), 2);
  ASSERT_EQ(QMD.getDiagonalNeighborNodes(7).size(), 4);
  ASSERT_EQ(QMD.getDiagonalNeighborNodes(12).size(), 2);
  ASSERT_EQ(QMD.getDiagonalNeighborNodes(19).size(), 2);
  ASSERT_EQ(QMD.getDiagonalNeighborNodes(14).size(), 4);
  ASSERT_EQ(QMD.getDiagonalNeighborNodes(21).size(), 1);

  ASSERT_EQ(QMD.getDiagonalNeighborNodes(0)[0], 7);

  NeighborConnectionTest({0,2,12,14}, QMD.getDiagonalNeighborNodes(7), 7);
  NeighborConnectionTest({7,9}, QMD.getDiagonalNeighborNodes(2), 2);
  NeighborConnectionTest({7}, QMD.getDiagonalNeighborNodes(0), 0);
  NeighborConnectionTest({2, 14}, QMD.getDiagonalNeighborNodes(9), 9);
  NeighborConnectionTest({7, 19}, QMD.getDiagonalNeighborNodes(12), 12);
  NeighborConnectionTest({7,9,19,21}, QMD.getDiagonalNeighborNodes(14), 14);
  NeighborConnectionTest({12, 14}, QMD.getDiagonalNeighborNodes(19), 19);
  NeighborConnectionTest({14}, QMD.getDiagonalNeighborNodes(21), 21);

}


int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}