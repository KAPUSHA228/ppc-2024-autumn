// Copyright 2023 Nesterov Alexander
#include "mpi/tsatsyn_a_topology_torus_grid/include/ops_mpi.hpp"

#include <algorithm>
#include <chrono>
#include <functional>
#include <random>
#include <string>
#include <thread>
#include <vector>
using namespace std::chrono_literals;

std::vector<int> tsatsyn_a_topology_torus_grid_mpi::getRandomVector(int sz) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::vector<int> vec(sz);
  for (int i = 0; i < sz; i++) {
    vec[i] = gen() % 100;
  }
  return vec;
}

// bool tsatsyn_a_topology_torus_grid_mpi::TestMPITaskSequential::pre_processing() {
//  internal_order_test();
//  // Init vectors
//  input_ = std::vector<int>(taskData->inputs_count[0]);
//  auto* tmp_ptr = reinterpret_cast<int*>(taskData->inputs[0]);
//  for (unsigned i = 0; i < taskData->inputs_count[0]; i++) {
//    input_[i] = tmp_ptr[i];
//  }
//  // Init value for output
//  res = 0;
//  return true;
//}
//
// bool tsatsyn_a_topology_torus_grid_mpi::TestMPITaskSequential::validation() {
//  internal_order_test();
//  // Check count elements of output
//  return taskData->outputs_count[0] == 1;
//}
//
// bool tsatsyn_a_topology_torus_grid_mpi::TestMPITaskSequential::run() {
//  internal_order_test();
//  if (ops == "+") {
//    res = std::accumulate(input_.begin(), input_.end(), 0);
//  } else if (ops == "-") {
//    res = -std::accumulate(input_.begin(), input_.end(), 0);
//  } else if (ops == "max") {
//    res = *std::max_element(input_.begin(), input_.end());
//  }
//  return true;
//}
//
// bool tsatsyn_a_topology_torus_grid_mpi::TestMPITaskSequential::post_processing() {
//  internal_order_test();
//  reinterpret_cast<int*>(taskData->outputs[0])[0] = res;
//  return true;
//}
void myRecv() {}

void mySend(boost::mpi::communicator& world, int source_rank, int current_rank, int dest_rank, int cols, int rows,
            std::map<std::string, int> neighbors, std::vector<int>& input_data) {
  int source_row_pos, current_row_pos, dest_row_pos, source_col_pos, current_col_pos, dest_col_pos;
  source_row_pos = source_rank / cols;
  current_row_pos = current_rank / cols;
  dest_row_pos = dest_rank / cols;
  source_col_pos = source_rank % cols;
  current_col_pos = current_rank % cols;
  dest_col_pos = dest_rank % cols;
  // std::cout << "zalupa"<< source_rank << current_rank << dest_rank << std::endl;
  if (world.rank() == 0) {
    std::cout << "input size: " << input_data.size() << std::endl;
    std::cout << "source_row_pos: " << source_row_pos << std::endl;
    std::cout << "dest_row_pos: " << dest_row_pos << std::endl;
    std::cout << "source_col_pos: " << source_col_pos << std::endl;
    std::cout << "dest_col_pos: " << dest_col_pos << std::endl;
  }
  std::cout << world.rank() << " current_row_pos: " << current_row_pos << " current_col_pos: " << current_col_pos
            << std::endl;

  std::vector<int> copy;
  if (source_rank == dest_rank) {  // рефлексивность
    std::cout << "LOХ";
    return;
  } else if (current_rank == dest_rank) {  // откуда придёт вызов для последнего
    if (source_col_pos == dest_col_pos) {  // если мы в одной колонке с нужным процессом
      if (rows % 2 == 0) {                 // четное ли количество рядов
        if (abs(dest_row_pos - source_row_pos) > ((rows + 1) / 2)) {  // ф1
          source_row_pos < dest_row_pos ? world.recv(neighbors["up"], 0, input_data)
                                        : world.recv(neighbors["down"], 0, input_data);
        } else {
          source_row_pos < dest_row_pos ? world.recv(neighbors["down"], 0, input_data)
                                        : world.recv(neighbors["up"], 0, input_data);
        }
      } else {
        if (abs(dest_row_pos - source_row_pos) > ((rows - 1) / 2)) {  // ф2
          source_row_pos < dest_row_pos ? world.recv(neighbors["up"], 0, input_data)
                                        : world.recv(neighbors["down"], 0, input_data);
        } else {
          source_row_pos < dest_row_pos ? world.recv(neighbors["down"], 0, input_data)
                                        : world.recv(neighbors["up"], 0, input_data);
        }
      }
    } else {
      if (cols % 2 == 0) {  // если не в одной колонке то тоже самое но для колонок
        if (abs(dest_col_pos) - source_col_pos > ((cols + 1) / 2)) {  // ф1
          source_col_pos < dest_col_pos ? world.recv(neighbors["left"], 0, input_data)
                                        : world.recv(neighbors["right"], 0, input_data);

        } else {
          source_col_pos < dest_col_pos ? world.recv(neighbors["right"], 0, input_data)
                                        : world.recv(neighbors["left"], 0, input_data);
        }
      } else {
        if (abs(dest_col_pos - source_col_pos) > ((cols - 1) / 2)) {  // ф2
          source_col_pos < dest_col_pos ? world.recv(neighbors["left"], 0, input_data)
                                        : world.recv(neighbors["right"], 0, input_data);

        } else {
          source_col_pos < dest_col_pos ? world.recv(neighbors["right"], 0, input_data)
                                        : world.recv(neighbors["left"], 0, input_data);
        }
      }
    }
    std::cout << "Answer: " << input_data.size() << std::endl;
    return;
  }
  //(rows+1)/2+1
  //(rows-1)/2+1
  else if (current_rank != source_rank) {  // когда текущий не начальный но и не конечный
    int delta_row, delta_col;
    delta_row = dest_row_pos - current_row_pos;
    delta_col = dest_col_pos - current_col_pos;
    // видимо забыл реализовать мысль процессы больше/меньше целевого не получают recv
    if (delta_row != 0) {
      if (rows % 2 == 0) {
        if (delta_row < 0) {  // цель выше текущего положения
          if ((abs(delta_row) < (rows + 1) / 2) && (world.rank() > dest_row_pos) &&
              (world.rank() < source_row_pos)) {  // напрямую
            world.recv(neighbors["down"], 0, copy);
            world.send(neighbors["up"], 0, copy);
          }
          if ((abs(delta_row) > (rows + 1) / 2) && (world.rank() < dest_row_pos) &&
              (world.rank() > source_row_pos)) {  // в обход через края
            world.recv(neighbors["up"], 0, copy);
            world.send(neighbors["down"], 0, copy);
          }
        } else {                                  // цель ниже текущего положения
          if (abs(delta_row) < (rows + 1) / 2) {  // дописать
            world.recv(neighbors["up"], 0, copy);
            world.send(neighbors["down"], 0, copy);
          } else {
            world.recv(neighbors["down"], 0, copy);
            world.send(neighbors["up"], 0, copy);
          }
        }
      } else {
        if (delta_row < 0) {
          if (abs(delta_row) < (rows - 1) / 2) {  // дописать
            world.recv(neighbors["down"], 0, copy);
            world.send(neighbors["up"], 0, copy);
          } else {
            world.recv(neighbors["up"], 0, copy);
            world.send(neighbors["down"], 0, copy);
          }
        } else {
          if (abs(delta_row) < (rows - 1) / 2) {  // дописать
            world.recv(neighbors["up"], 0, copy);
            world.send(neighbors["down"], 0, copy);
          } else {
            world.recv(neighbors["down"], 0, copy);
            world.send(neighbors["up"], 0, copy);
          }
        }
      }
      // return;
    } else if (delta_col != 0) {
      if (cols % 2 == 0) {    // 2 разных вида половин
        if (delta_col < 0) {  // цель левее текущего положения
          if (abs(delta_col) < (cols + 1) / 2) {
            world.recv(neighbors["right"], 0, copy);
            world.send(neighbors["left"], 0, copy);
          } else {
            world.recv(neighbors["left"], 0, copy);
            world.send(neighbors["right"], 0, copy);
          }
        } else {
          if (abs(delta_col) < (cols + 1) / 2) {
            world.recv(neighbors["left"], 0, copy);
            world.send(neighbors["right"], 0, copy);
          } else {
            world.recv(neighbors["right"], 0, copy);
            world.send(neighbors["left"], 0, copy);
          }
        }
      } else {
        if (delta_col < 0) {
          // не забыть что сначала иду по рядам а потом ищу нужную колонку
          if (abs(delta_col) < (cols - 1) / 2) {
            world.recv(neighbors["right"], 0, copy);
            world.send(neighbors["left"], 0, copy);
          } else {
            world.recv(neighbors["left"], 0, copy);
            world.send(neighbors["right"], 0, copy);
          }
        } else {
          if (abs(delta_col) < (cols - 1) / 2) {
            world.recv(neighbors["left"], 0, copy);
            world.send(neighbors["right"], 0, copy);
          } else {
            world.recv(neighbors["right"], 0, copy);
            world.send(neighbors["left"], 0, copy);
          }
        }
      }
    }
  } else {  // когда текущий равен начальному
    if (current_row_pos < dest_row_pos) {
      std::cout << "eban" << std::endl;
      world.send(neighbors["down"], 0, input_data);
    } else if (current_row_pos > dest_row_pos) {
      std::cout << "ebun" << std::endl;
      world.send(neighbors["up"], 0, input_data);
    } else {
      std::cout << "eben" << std::endl;
      world.send(neighbors["right"], 0, input_data);
    }
  }
  return;
}
void myBroadcast(boost::mpi::communicator& world, std::map<std::string, int> neighbors, int rows, int cols,
                 bool is_main_magistralle, int col_pos, int row_pos, std::vector<int>& input_data) {
  // Разделение данных между процессами
  int delta;
  if (world.rank() == 0) {
    int sizeinput = input_data.size();
    // std::cout << "sizeinput " << sizeinput << std::endl;
    int limit = 10000;
    delta = sizeinput < limit ? 1 : (std::ceil(sizeinput / limit) + 1);
    for (int proc = 1; proc < world.size(); proc++) {
      world.send(proc, 0, delta);
    }
    std::cout << "delta " << delta << std::endl;

    for (int i = 0; i < delta; i++) {
      std::vector<int> local_input_data;
      int endOfCycle = input_data.size() - limit * (i) >= limit ? limit * (i + 1) : input_data.size();
      // std::cout << "limit*delta " << limit * i << std::endl;
      // std::cout << "endofCycle " << endOfCycle << std::endl;

      for (int j = i * limit; j < endOfCycle; j++) local_input_data.push_back(input_data[j]);
      std::cout << "local " << i << " " << local_input_data.size() << std::endl;
      //  набор частных случаев которые не поддаются шаблону

      // квадрат 2х2
      if ((rows == 2) && (cols == 2)) {
        // vec, taskData->inputs_count[0]);
        world.send(neighbors["right"], 0, local_input_data);
        world.send(neighbors["down"], 0, local_input_data);
      }
      // колонок 2
      else if ((rows > 2) && (cols == 2)) {
        world.send(neighbors["right"], 0, local_input_data);
        world.send(neighbors["down"], 0, local_input_data);
        world.send(neighbors["up"], 0, local_input_data);
      }  // частный случай для 2х1
      else if ((rows == 1) && (cols == 2)) {
        world.send(neighbors["right"], 0, local_input_data);
      }  // всё остальное
      else if ((cols > 2) && (rows > 2)) {
        world.send(neighbors["left"], 0, local_input_data);
        world.send(neighbors["right"], 0, local_input_data);
        world.send(neighbors["down"], 0, local_input_data);
        world.send(neighbors["up"], 0, local_input_data);
      }  // рядов 2
      else if ((cols > 2) && (rows == 2)) {
        world.send(neighbors["right"], 0, local_input_data);
        world.send(neighbors["down"], 0, local_input_data);
        world.send(neighbors["left"], 0, local_input_data);
      }  // шаблон для 1х1,Nx1
      else {
        for (auto proc : neighbors) {
          if (proc.second != world.rank()) {
            world.send(proc.second, 0, local_input_data);
          }
        }
      }
    }

  } else {
    world.recv(0, 0, delta);
    // std::cout << "process " << world.rank() << " " << input_data.size() << std::endl;
    std::vector<int> copy;
    // если на главной шине (0-ая колонка)
    for (int i = 0; i < delta; i++) {
      if (is_main_magistralle) {
        // если число элементов в колонке чётное(кол-во рядов)
        if (rows % 2 == 0) {
          if (rows == 2) {
            world.recv(neighbors["up"], 0, copy);
            input_data.insert(input_data.end(), copy.begin(), copy.end());
            if (neighbors["right"] != neighbors["left"]) {
              world.send(neighbors["left"], 0, copy);
              world.send(neighbors["right"], 0, copy);
            } else {
              world.send(neighbors["left"], 0, copy);
            }
          } else {
            if (row_pos < (rows + 1) / 2) {  // то до чего должен дойти левый край(невключительно)
              world.recv(neighbors["up"], 0, copy);
              world.send(neighbors["down"], 0, copy);
            } else if (row_pos > ((rows + 1) / 2) + 1) {  // то до чего должен дойти правый край(невключительно)
              world.recv(neighbors["down"], 0, copy);
              world.send(neighbors["up"], 0, copy);
            } else if (row_pos == (rows + 1) / 2) {
              world.recv(neighbors["up"], 0, copy);
            } else if (row_pos == ((rows + 1) / 2) + 1) {
              world.recv(neighbors["down"], 0, copy);
            }
            input_data.insert(input_data.end(), copy.begin(), copy.end());
            if (neighbors["right"] != neighbors["left"]) {
              world.send(neighbors["left"], 0, copy);
              world.send(neighbors["right"], 0, copy);
            } else {
              world.send(neighbors["left"], 0, copy);
            }
          }
        }
        // если число элементов в колонке нечётное(кол-во рядов)
        else {
          if (rows != 1) {
            if (row_pos < (rows - 1) / 2) {  // то до чего должен дойти левый край(невключительно)
              world.recv(neighbors["up"], 0, copy);
              world.send(neighbors["down"], 0, copy);
            } else if (row_pos > ((rows - 1) / 2) + 1) {  // то до чего должен дойти правый край(невключительно)
              world.recv(neighbors["down"], 0, copy);
              world.send(neighbors["up"], 0, copy);
            } else if (row_pos == (rows - 1) / 2) {
              world.recv(neighbors["up"], 0, copy);
            } else if (row_pos == ((rows - 1) / 2) + 1) {
              world.recv(neighbors["down"], 0, copy);
            }
            input_data.insert(input_data.end(), copy.begin(), copy.end());
          }
          if (cols != 1) {
            if (neighbors["right"] != neighbors["left"]) {
              world.send(neighbors["left"], 0, copy);
              world.send(neighbors["right"], 0, copy);
            } else {
              world.send(neighbors["left"], 0, copy);
            }
          }
        }
      }
      // если не на главной шине
      else {
        // если число элементов в ряду чётное(кол-во колонок)
        if (cols % 2 == 0) {
          if (col_pos < (cols + 1) / 2) {
            world.recv(neighbors["left"], 0, copy);
            world.send(neighbors["right"], 0, copy);
          } else if (col_pos > ((cols + 1) / 2) + 1) {
            world.recv(neighbors["right"], 0, copy);
            world.send(neighbors["left"], 0, copy);
          } else if (col_pos == (cols + 1) / 2) {
            world.recv(neighbors["left"], 0, copy);
          } else if (col_pos == (cols + 1) / 2 + 1) {
            world.recv(neighbors["right"], 0, copy);
          }
          input_data.insert(input_data.end(), copy.begin(), copy.end());
        }
        // если число элементов в ряду нечётное(кол-во колонок)
        else {
          if (col_pos < (cols - 1) / 2) {
            world.recv(neighbors["left"], 0, copy);
            world.send(neighbors["right"], 0, copy);
          } else if (col_pos > ((cols - 1) / 2) + 1) {
            world.recv(neighbors["right"], 0, copy);
            world.send(neighbors["left"], 0, copy);
          } else if (col_pos == (cols - 1) / 2) {
            world.recv(neighbors["left"], 0, copy);
          } else if (col_pos == ((cols - 1) / 2) + 1) {
            world.recv(neighbors["right"], 0, copy);
          }
          input_data.insert(input_data.end(), copy.begin(), copy.end());
        }
      }
    }
  }
}
int* hasDivisors(int k) {
  int* mas = new int[k];
  for (int i = 0; i < k; i++) {
    mas[i] = -1;
  }
  int j = 0;
  for (int i = 2; i < k; i++) {
    if (k % i == 0) {
      mas[j] = i;
      j++;
    }
  }
  return mas[0] == -1 ? nullptr : mas;
}
bool tsatsyn_a_topology_torus_grid_mpi::TestMPITaskParallel::pre_processing() {
  internal_order_test();
  std::cout << "it was process No" << world.rank() << std::endl;

  // создание сетки в топологии
  std::map<std::string, int> neighbors;
  int rows, cols, row_pos, col_pos;
  bool is_main_magistralle;
  if (world.rank() == 0) {
    input_data.resize(taskData->inputs_count[0]);
    auto* tempPtr = reinterpret_cast<int*>(taskData->inputs[0]);
    std::copy(tempPtr, tempPtr + taskData->inputs_count[0], input_data.begin());
    vec = reinterpret_cast<int*>(taskData->inputs[0]);
    if (hasDivisors(world.size()) == nullptr) {
      cols = world.size();
      rows = 1;
    } else {
      int* mas_copy = hasDivisors(world.size());
      int i = 0;
      while (mas_copy[i] != -1) {
        i++;
      }
      std::random_device dev;
      std::mt19937 gen(dev());
      int randIndex = gen() % (i - 1 + 1) + 1;
      rows = mas_copy[randIndex - 1];
      cols = world.size() / rows;
    }  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (int proc = 1; proc < world.size(); proc++) {
      world.send(proc, 0, rows);
      world.send(proc, 1, cols);
    }
  } else {
    world.recv(0, 0, rows);
    world.recv(0, 1, cols);
  }
  if (world.rank() == 0) {
    std::cout << "col: " << cols << std::endl;
    std::cout << "row: " << rows << std::endl;
  }
  world.barrier();
  row_pos = world.rank() / cols;
  col_pos = world.rank() % cols;
  is_main_magistralle = (col_pos == 0);
  /* std::cout << "Process " << world.rank() << " with magistralle " << is_main_magistralle << " with coords " <<
     row_pos
             << ":" << col_pos << std::endl;*/
  auto toGetNeighbor = [&](int r, int c) -> int {
    int neighbor_rank = r * cols + c;
    return (neighbor_rank < world.size()) ? neighbor_rank : -1;  // -1 означает, что соседа нет(уже неактуально)
  };

  // прописка соседей
  neighbors["down"] = (row_pos == rows - 1) ? toGetNeighbor(0, col_pos) : toGetNeighbor(row_pos + 1, col_pos);
  neighbors["left"] = (col_pos == 0) ? toGetNeighbor(row_pos, cols - 1) : toGetNeighbor(row_pos, col_pos - 1);
  neighbors["right"] = (col_pos == cols - 1) ? toGetNeighbor(row_pos, 0) : toGetNeighbor(row_pos, col_pos + 1);
  neighbors["up"] = (row_pos == 0) ? toGetNeighbor(rows - 1, col_pos) : toGetNeighbor(row_pos - 1, col_pos);

  // myBroadcast(world,neighbors, rows, cols, is_main_magistralle, col_pos, row_pos, input_data);
  mySend(world, 0, world.rank(), 1, cols, rows, neighbors, input_data);
  /*for (const auto& neighbor : neighbors) {
    std::cout << "Neighbors of " << world.rank() << ": " << neighbor.first << " , " << neighbor.second << std::endl;
    world.barrier();
  }*/
  std::cout << "size of " << world.rank() << " input : " << input_data.size() << std::endl;
  world.barrier();
  return true;
}

bool tsatsyn_a_topology_torus_grid_mpi::TestMPITaskParallel::validation() {
  internal_order_test();
  // if (world.rank() == 0) {
  //   // Check count elements of output
  //   return taskData->outputs_count[0] == 1;
  // }
  return true;
}

bool tsatsyn_a_topology_torus_grid_mpi::TestMPITaskParallel::run() {
  internal_order_test();
  /*int local_res;
  if (ops == "+") {
    local_res = std::accumulate(local_input_.begin(), local_input_.end(), 0);
  } else if (ops == "-") {
    local_res = -std::accumulate(local_input_.begin(), local_input_.end(), 0);
  } else if (ops == "max") {
    local_res = *std::max_element(local_input_.begin(), local_input_.end());
  }

  if (ops == "+" || ops == "-") {
    reduce(world, local_res, res, std::plus(), 0);
  } else if (ops == "max") {
    reduce(world, local_res, res, boost::mpi::maximum<int>(), 0);
  }*/
  return true;
}

bool tsatsyn_a_topology_torus_grid_mpi::TestMPITaskParallel::post_processing() {
  internal_order_test();
  /*if (world.rank() == 0) {
    reinterpret_cast<int*>(taskData->outputs[0])[0] = res;
  }*/
  return true;
}
