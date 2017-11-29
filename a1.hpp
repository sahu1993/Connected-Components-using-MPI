/*  YOUR_FIRST_NAME - Shivam
 *  YOUR_LAST_NAME - Sahu
 *  YOUR_UBIT_NAME - Ssahu3
 */

#ifndef A1_HPP
#define A1_HPP

#include <vector>
#include <mpi.h>
#include <unistd.h>


int connected_components(std::vector<signed char>& A, int n, int q, const char* out, MPI_Comm comm) {
    // ...
    int size = n/q ;
    std::vector<int> p;
    std::vector<int> m(size*size,0);
    std::vector<int> helper(size*size,0);
    std::vector<int> Q(size*size,0);
    std::vector<int> P_Prime(size*size,0);
    std::vector<int> TreeHanging(size*size,0);
    std::vector<int> P_Final;



    int rank, comm1 ;
    MPI_Comm_rank(comm, &rank);
    int col = rank % q;
    int row = rank / q;
    // std::cout<<"row: "<<row<<'\t';
    // std::cout<<"col: "<<col<<'\n';

    // std::cout<<"size: "<<size<<'\t';

    MPI_Barrier(comm);
    sleep(rank);
    for(int i =0 ; i< A.size(); i = i+size){
      for(int j = i; j< i+size ; j++)
      {
        if(static_cast<int>(A[j]) == 1){
          p.push_back((j/size) + (size*row));
        }
        else
        {
          p.push_back(0);
        }
      }
    }
    MPI_Barrier(comm);

    // sleep(rank);
    // std::cout<<"P :\n";
    // std::cout<<"Rank :"<<rank<<"\n";
    // for (int i = 0; i < size; ++i) {
    //     for (int j = 0; j < size; ++j)
    //     std::cout << p[i * size + j] << " ";
    //     std::cout << std::endl;
    // }
    // MPI_Barrier(comm);
    // sleep(rank);

  std::vector<int> max(size,0);
    for(int i =0 ; i < size; i++){
      for(int j = i; j< p.size() ; j = j + size )
      {
        if(p[j] > max[i]){
          max[i] = p[j];
        }
      }
    }

    for(int i = 0; i < size; i++)
    {
      for(int j = i; j< m.size() ; j = j + size ){
          m[j] = max[i];
      }
    }

    // MPI_Barrier(comm);
    // sleep(rank);
    // std::cout<<"M :\n";
    // std::cout<<"Rank :"<<rank<<"\n";
    // for (int i = 0; i < size; ++i) {
    //     for (int j = 0; j < size; ++j)
    //     std::cout << m[i * size + j] << " ";
    //     std::cout << std::endl;
    // }

    MPI_Barrier(comm);
    MPI_Comm_split(comm,col,rank,&comm1);
    MPI_Allreduce(m.data(),p.data(),(size*size),MPI_INT,MPI_MAX,comm1);
    MPI_Comm_free(&comm1);
    MPI_Barrier(comm);

    // sleep(rank);
    // std::cout<<"P1 : \n";
    // std::cout<<"Rank :"<<rank<<"\n";
    // for (int i = 0; i < size; ++i) {
    //     for (int j = 0; j < size; ++j)
    //     std::cout << p[i * size + j] << " ";
    //     std::cout << std::endl;
    // }
    // MPI_Barrier(comm);
    // sleep(rank);

    for(int i=0; i< A.size(); i++ )
    {
      if(static_cast<int>(A[i]) == 1)
      {
        helper[i] = p[i];
      }
    }

    MPI_Barrier(comm);

    //sleep(rank);
    // std::cout<<"Helper : \n";
    // std::cout<<"Rank :"<<rank<<"\n";
    // for (int i = 0; i < size; ++i) {
    //     for (int j = 0; j < size; ++j)
    //     std::cout << helper[i * size + j] << " ";
    //     std::cout << std::endl;
    // }
    // MPI_Barrier(comm);
    // sleep(rank);

    std::vector<int> max1(size,0);
      for(int i =0 ; i < size; i++){
        for(int j = i*size; j < (i*size)+size ; j++ )
        {
          if(helper[j] > max1[i]){
            max1[i] = helper[j];
          }
        }
      }

      for(int i =0 ; i < size; i++){
        for(int j = i*size; j < (i*size)+size ; j++ ){
            helper[j] = max1[i];
        }
      }

      // MPI_Barrier(comm);
      // sleep(rank);
      //
      // std::cout<<"Helper1 : \n";
      // std::cout<<"Rank :"<<rank<<"\n";
      // for (int i = 0; i < size; ++i) {
      //     for (int j = 0; j < size; ++j)
      //     std::cout << helper[i * size + j] << " ";
      //     std::cout << std::endl;
      // }
      // MPI_Barrier(comm);
      // sleep(rank);

      MPI_Barrier(comm);
      MPI_Comm_split(comm,row,rank,&comm1);
      MPI_Allreduce(helper.data(),Q.data(),(size*size),MPI_INT,MPI_MAX,comm1);
      MPI_Comm_free(&comm1);
      MPI_Barrier(comm);

      // sleep(rank);
      // std::cout<<"Q : \n";
      // std::cout<<"Rank :"<<rank<<"\n";
      // for (int i = 0; i < size; ++i) {
      //     for (int j = 0; j < size; ++j)
      //     std::cout << Q[i * size + j] << " ";
      //     std::cout << std::endl;
      // }
      // MPI_Barrier(comm);
      // sleep(rank);

      for(int i=0; i<size; i++){
        for(int j=0; j<size; j++)
        {
          if(Q[i*size + j] == j + (size*col))
          {
            m[i*size + j] = p[i*size + j];
          }
          else
          {
            m[i*size + j] = 0;
          }
        }
      }

      MPI_Barrier(comm);
      // sleep(rank);
      //
      // std::cout<<"M1 : \n";
      // std::cout<<"Rank :"<<rank<<"\n";
      // for (int i = 0; i < size; ++i) {
      //     for (int j = 0; j < size; ++j)
      //     std::cout << m[i * size + j] << " ";
      //     std::cout << std::endl;
      // }
      // MPI_Barrier(comm);
      // sleep(rank);

      std::vector<int> max2(size,0);
        for(int i =0 ; i < size; i++){
          for(int j = i*size; j < (i*size)+size ; j++ )
          {
            if(m[j] > max2[i]){
              max2[i] = m[j];
            }
          }
        }

        for(int i =0 ; i < size; i++){
          for(int j = i*size; j < (i*size)+size ; j++ ){
              m[j] = max2[i];
          }
        }

        MPI_Barrier(comm);
        MPI_Comm_split(comm,row,rank,&comm1);
        MPI_Allreduce(m.data(),P_Prime.data(),(size*size),MPI_INT,MPI_MAX,comm1);
        MPI_Comm_free(&comm1);
        MPI_Barrier(comm);

        // sleep(rank);
        // std::cout<<"P_Prime : \n";
        // std::cout<<"Rank :"<<rank<<"\n";
        // for (int i = 0; i < size; ++i) {
        //     for (int j = 0; j < size; ++j)
        //     std::cout << P_Prime[i * size + j] << " ";
        //     std::cout << std::endl;
        // }
        // MPI_Barrier(comm);
        // sleep(rank);

        for(int i=0; i<size; i++){
          for(int j=0; j<size; j++)
          {
            if(p[j+(size * i)] == i + ((rank / q) * size)){
                m[j+(size * i)] = P_Prime[j+(size * i)];
            }else{
                m[j+(size * i)] = 0;
            }
          }
        }

        MPI_Barrier(comm);
        // sleep(rank);
        //
        // std::cout<<"M2 : \n";
        // std::cout<<"Rank :"<<rank<<"\n";
        // for (int i = 0; i < size; ++i) {
        //     for (int j = 0; j < size; ++j)
        //     std::cout << m[i * size + j] << " ";
        //     std::cout << std::endl;
        // }
        // MPI_Barrier(comm);
        // sleep(rank);


        std::vector<int> max3(size,0);
          for(int i =0 ; i < size; i++){
            for(int j = i*size; j < (i*size)+size ; j++ )
            {
              if(m[j] > max3[i]){
                max3[i] = m[j];
              }
            }
          }

          for(int i =0 ; i < size; i++){
            for(int j = i*size; j < (i*size)+size ; j++ ){
                m[j] = max3[i];
            }
          }

          MPI_Barrier(comm);
          MPI_Comm_split(comm,row,rank,&comm1);
          MPI_Allreduce(m.data(),TreeHanging.data(),(size*size),MPI_INT,MPI_MAX,comm1);
          MPI_Comm_free(&comm1);
          MPI_Barrier(comm);

          // sleep(rank);
          // std::cout<<"TreeHanging : \n";
          // std::cout<<"Rank :"<<rank<<"\n";
          // for (int i = 0; i < size; ++i) {
          //     for (int j = 0; j < size; ++j)
          //     std::cout << TreeHanging[i * size + j] << " ";
          //     std::cout << std::endl;
          // }
          // MPI_Barrier(comm);
          // sleep(rank);

          for(int i=0; i<size ; i++)
          {
            for(int j= 0; j<size ; j++)
            {
              if(TreeHanging[i * size + j] > P_Prime[i * size + j])
                P_Final.push_back(TreeHanging[i*size + j]);
              else
                P_Final.push_back(P_Prime[i*size + j]);
            }
          }
          MPI_Barrier(comm);

          sleep(rank);
          std::cout<<"P_Final : \n";
          std::cout<<"Rank :"<<rank<<"\n";
          for (int i = 0; i < size; ++i) {
              for (int j = 0; j < size; ++j)
              std::cout << P_Final[i * size + j] << " ";
              std::cout << std::endl;
          }
          sleep(rank);

    return -1;
} // connected_components

#endif // A1_HPP
