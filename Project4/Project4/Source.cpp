#define _CRT_SECURE_NO_WARNINGS
#include <mpi.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>
#include <vector>
using namespace std;


void level_1(MPI_Group world_group, int rank, int size);
void level_2(MPI_Group world_group, MPI_Comm dub_comm_world, int size, int rank);


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
	level_1(world_group, rank, size);

	level_2(world_group, dub_comm_world, size, rank);

	MPI_Group_free(&world_group);
	MPI_Comm_free(&dub_comm_world);
	MPI_Finalize();
	return 0;
}


void level_1(MPI_Group world_group, int rank, int size)
{
	if (size >= 7)
	{
		MPI_Group group_1, group_2, group_union;
		int rank1[] = { 1,2,5 }, rank2[] = { 1,3,4,6 };
		int rank_1, rank_2, group_rank, size_1, size_2, group_size;

		MPI_Group_incl(world_group, 3, rank1, &group_1);//group_1
		MPI_Group_rank(group_1, &rank_1);
		MPI_Group_size(group_1, &size_1);
		if(size_1!=0)
			if (rank_1 >= 0 && rank_1 <= 2)
				cout << "\nlevel1: Rank - " << rank << "; group_1 rank - " << rank_1;

		MPI_Group_incl(world_group, 4, rank2, &group_2);//group_2
		MPI_Group_rank(group_2, &rank_2);
		MPI_Group_size(group_2, &size_2);
		if(size_2 !=0)
			if (rank_2 >= 0 && rank_2 <= 3)
			    cout << "\nlevel1: Rank - " << rank << "; group_2 rank - " << rank_2;

		MPI_Group_union(group_1, group_2, &group_union);//group_union
		MPI_Group_rank(group_union, &group_rank);
		MPI_Group_size(group_union, &group_size);
		if (group_size == 0)
			cout << "\nlevel1: The groups didn't union";
		if (group_rank >= 0 && group_rank <= 5)
			cout << "\nlevel1: Rank - " << rank << "; group_union rank - " << group_rank;

		MPI_Group_free(&group_1);
		MPI_Group_free(&group_2);
		MPI_Group_free(&group_union);
	}
}


void level_2(MPI_Group world_group, MPI_Comm dub_comm_world, int size, int rank)
{
	if (size >= 8)
	{
		int rank3[] = { 0,2,4,5,7 }, group_size, group_rank, message;
		MPI_Comm world_comm, new_dub_comm_world;
		MPI_Group group_3;

		MPI_Group_incl(world_group, 5, rank3, &group_3);//group_3
		MPI_Comm_create(dub_comm_world, group_3, &world_comm);//create
		MPI_Group_size(group_3, &group_size);
		MPI_Group_rank(group_3, &group_rank);
		if (group_rank >= 0)
		{
			MPI_Comm_dup(world_comm, &new_dub_comm_world);//duplication
			cout << "\nLevel2  Rank - " << rank << "; group_3 rank - " << group_rank;
			if (group_rank == 3)
			{
				message = 3;
			}
			MPI_Bcast(&message, 1, MPI_INT, 3, new_dub_comm_world);
			if (group_rank != 3)
			cout << "\nLevel2 Rank - " << rank << "; group_3 rank - " << group_rank << ";  message:  " << message;

		}
		MPI_Group_free(&group_3);
		
	}
}