#define _CRT_SECURE_NO_WARNINGS
#include <mpi.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <vector>
#include <algorithm>    
#include <array>        
#include <random>       
#include <chrono>
using namespace std;

void level_1(MPI_Group world_group, MPI_Comm dub_comm_world, int rank, int size);
void  dataExchange( MPI_Comm comm, int rank, int s, int r);

int main(int argc, char *argv[])
{
	int rank, size;
	MPI_Comm dub_comm_world;
	MPI_Group world_group;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_dup(MPI_COMM_WORLD, &dub_comm_world);
	MPI_Comm_group(dub_comm_world, &world_group);
	
	level_1(world_group, dub_comm_world, rank, size);

	MPI_Group_free(&world_group);
	MPI_Comm_free(&dub_comm_world);
	MPI_Finalize();
	return 0;
}

void  dataExchange( MPI_Comm comm, int numberOfProcess, int s, int r)
{
        // c, s, r and so on... o p r s t - such names are acceptable, for example, for counters, temporary variables with short lifespan,
        // otherwise it's a good style when each variable has meaningful name, like numberOfProcess, etc.
        // This helps to read code faster.
        // Because it's hard to keep in mind what means s, what means r and so on...
	MPI_Status status;
	string group;
	int c;
	MPI_Recv(&c, 1, MPI_INT, s, 10, comm, &status);
	vector <char> str;
	str.resize(c);
	MPI_Recv(str.data(), c, MPI_CHAR, s, 0, comm, &status);
	copy(str.begin(), str.end(), back_inserter(group));
	group.append(to_string(numberOfProcess));
	c = group.length();
	MPI_Ssend(&c, 1, MPI_INT, r, 10, comm);
	MPI_Ssend(const_cast<char*>(group.c_str()), c, MPI_CHAR, r, 0, comm);
	group.clear();
}

void level_1(MPI_Group world_group, MPI_Comm dub_comm_world,  int rank, int size)
{
	
	if (size == 8)
	{
		MPI_Comm world_comm, b_comm_world;
		MPI_Group group_1, group_2;
		MPI_Status status;
		string group1, group2;
		int rank_1, rank_2, size_1, size_2;
		int rank1[4], rank2[4], c, p1, p2;
		if (rank == 0)
		{
			array <int, 8> ranks{ 0, 1, 2, 3, 4, 5, 6, 7 };
			shuffle(ranks.begin(), ranks.end(), std::random_device());
			for (int i = 0; i < 4; i++)  rank1[i] = ranks[i];
			for (int i = 4; i < 8; i++)  rank2[i - 4] = ranks[i];
			int p1 = ranks[0];
		}
		MPI_Bcast(&rank1, 4, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(&rank2, 4, MPI_INT, 0, MPI_COMM_WORLD);
		
		MPI_Group_incl(world_group, 4, rank1, &group_1);
		MPI_Comm_create(dub_comm_world, group_1, &world_comm);//create
		MPI_Group_rank(group_1, &rank_1);
		MPI_Group_size(group_1, &size_1);
		MPI_Group_incl(world_group, 4, rank2, &group_2);
		MPI_Comm_create(dub_comm_world, group_2, &b_comm_world);//create
		MPI_Group_rank(group_2, &rank_2);
		MPI_Group_size(group_2, &size_2);

                // Seems like this big if() scope and the next one are doing almost the same work,
                // except rank numbers and resulting strings exchange at the end.
                // It is better to move similar functionality out to separate routine, than just make copy-paste.
		if (size_1 != 0) {
			if (rank_1==0)
			{
				int p2 = rank2[0];
				group1.append(to_string(rank));
				c = group1.length();
				MPI_Ssend(&c, 1, MPI_INT, 1, 10, world_comm);
				MPI_Ssend(const_cast<char*>(group1.c_str()), c, MPI_CHAR, 1, 0, world_comm);
				group1.clear();

                                // The same as the previous note: next 5 lines can be moved out to separate function, smth like receiveString()
                                // This helps to keep code clean and easier for reading.
				MPI_Recv(&c, 1, MPI_INT, 3, 10, world_comm, &status);
				vector <char> str;
				str.resize(c);
				MPI_Recv(str.data(), c, MPI_CHAR, 3, 0, world_comm, &status);
				copy(str.begin(), str.end(), back_inserter(group1));
				cout << "\n Ranks first group: " << group1;
				str.clear();

				c = group1.length();
				MPI_Ssend(&c, 1, MPI_INT, p2, 10, dub_comm_world);
				MPI_Ssend(const_cast<char*>(group1.c_str()), c, MPI_CHAR, p2, 0, dub_comm_world);

				string massage2;
				MPI_Recv(&c, 1, MPI_INT, p2, 10, dub_comm_world, &status);
				str.resize(c);
				MPI_Recv(str.data(), c, MPI_CHAR, p2, 0, dub_comm_world, &status);
				copy(str.begin(), str.end(), back_inserter(massage2));
				cout << "\n Massages from the second group: " << massage2;
			}
			if (rank_1 == 1) dataExchange(world_comm, rank, 0, 2);

			if (rank_1 == 2) dataExchange(world_comm, rank, 1, 3);

			if (rank_1 == 3) dataExchange(world_comm, rank, 2, 0);		
		}
		
		if (size_2 != 0) {
			if (rank_2 == 0)
			{
				int p1 = rank1[0];
				group2.append(to_string(rank));
				c = group2.length();
				MPI_Ssend(&c, 1, MPI_INT, 1, 10, b_comm_world);
				MPI_Ssend(const_cast<char*>(group2.c_str()), c, MPI_CHAR, 1, 0, b_comm_world);
				group2.clear();

				MPI_Recv(&c, 1, MPI_INT, 3, 10, b_comm_world, &status);
				vector <char> str;
				str.resize(c);
				MPI_Recv(str.data(), c, MPI_CHAR, 3, 0, b_comm_world, &status);
				copy(str.begin(), str.end(), back_inserter(group2));
				cout << "\n Ranks second group: " << group2;

				str.clear();
				string massage1; 
				MPI_Recv(&c, 1, MPI_INT, p1, 10, dub_comm_world, &status);
				str.resize(c);
				MPI_Recv(str.data(), c, MPI_CHAR, p1, 0, dub_comm_world, &status);
				copy(str.begin(), str.end(), back_inserter(massage1));
				cout << "\n Massages from the first group: " << massage1;

                c = group2.length();
				MPI_Ssend(&c, 1, MPI_INT, p1, 10, dub_comm_world);
				MPI_Ssend(const_cast<char*>(group2.c_str()), c, MPI_CHAR, p1, 0, dub_comm_world);
			}
			if (rank_2 == 1) dataExchange(b_comm_world, rank, 0, 2);

			if (rank_2 == 2) dataExchange(b_comm_world, rank, 1, 3);

			if (rank_2 == 3) dataExchange(b_comm_world, rank, 2, 0);
		}
	}
}



