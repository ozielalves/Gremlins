/**
 * @file main.hpp
 * @version 1.0
 * @since Jun, 22. 
 * @date Jun, 27.
 * @author Oziel Alves (ozielalves@ufrn.edu.br)
 * @author Daniel Guerra (daniel.guerra13@hotmail.com)
 * @title Driver
 */

#include <iostream>
// #include <vector> // NÃO PODE - Deletar
#include <fstream>
#include <cstdlib>	// rand_r()
#include <random>	// std::random_device
#include <cmath>	// std::ceil
#include <cassert>	// assert
#include <chrono>	// std::chrono
#include <ctime>	// std::time_t
#include <string>	// std::string
#include <queue>	// std::priority_queue

#include "../include/SLPool.hpp"
#include "../include/mempool_common.hpp"

typedef std::time_t tempo;
typedef std::string string;

using namespace gm;

/**
 * @brief Event Class
 */

class Event
/*{{{*/
{
	public:
        /**
         * @brief Event default constructor
         */
        Event( ) { /*Empty*/ }
    
        /**
         * @brief Event Attibution constructor
         * @param _p A pointer
         * @param _t A time-stamp
         */
        Event(void *_p, tempo _t) {
            m_ptr = _p;
            m_time = _t;
        }
    
        /**
         * @brief Gets the Event time-stamp
         * @return The time-stamp
         */
        tempo getTimeStamp( ) {
            return m_time;
        }
    
        /**
         * @brief Gets the pointer
         * @return The pointer
         */
        void *getMemoryPtr( ) {
            return m_ptr;
        }
    
        /**
         * @brief Overloads the < operator
         * @param _e The another event to be comparised
         * @return True or False, comparing by the time-stamp
         */
        bool operator<(const Event &_e) const {
            return m_time < _e.m_time;
        }
    
	private:
        void *m_ptr;	//!< The Event pointer
        tempo m_time;	//!< The Event time-stamp
};
/*}}}*/

/**
 * @brief Allocation test in a limited time
 * @param _pool A pointer to the pool to be used
 * @param _timeLimit The time limit
 */
void StoragePoolTest(StoragePool *_pool, tempo _timeLimit)
/*{{{*/
{
    // [1] Setup random numbers generator for memory size, say [100,2000] bytes.
    std::random_device rdm;
    auto ms_max = 2000u, ms_min = 100u;
    
    // [2] Setup random numbers generator for time intervals, say [1,100] units.
    auto ti_max = 100u, ti_min = 1u;
    
    // [3] Create the pirority queue std::priority_queue 'pq' and
    //     insert some events comprising the simulation.
    std::priority_queue< Event > pq;
    
    // Assuming there is a class Event that creates a pair address/time stamp.

    // Run simulation fot the time set by the client.
    for ( tempo t(0); t < _timeLimit; t++ ) {
        
        // Run while we have events pending or time to run.
        while ( !pq.empty( ) ){
            
            // Access the event with the smallest time-stamp.
            Event ev = pq.top( );
            
            // Do nothing. Still some time left.
            if ( ev.getTimeStamp( ) > t ) break;
            
            // When we got here, the top event has run out of time.
            // Remove event from priority queue.
            pq.pop( );
            
            // Calling free operator.
            _pool->Free( ev.getMemoryPtr( ) );
        }
        
        auto memorySize = rdm( )%( ms_max - ms_min + 1 ) + ms_min;
        void *const add = _pool->Allocate(memorySize);
        auto timeSpent = rdm( )%( ti_max - ti_min + 1 ) + ti_min;
        
        // Set time stamp some time from now.
        tempo releaseTime = t + timeSpent;
        
        // Creating a new simulation event.
        pq.push( Event(add, releaseTime) );
        
        // Show Memory Map on Screen.
        _pool->view( );
    }
}

/*}}}*/

int main(/* int argc, char **argv */)
{
/*{{{*/
	std::cout << "\n\e[34;1m>>>Subtitles:\e[0m\n"
			  << "\t\e[30;1m+: Avaiable area. Free area.\n"
			  << "\t-: Occupied area. Already in use.\n"
			  << "\t*: Representation of free blocks. Avaiable for use.\n"
			  << "\t#: Representation of occupied blocks.\e[0m\n\n\n";
	//TODO...

	/*{
		StoragePool *pool = new gm::SLPool(300);

    	int *allocs[7];

    	// Shows the initial state from memory pool
    	std::cout << "~ Current State ~\n" << string(4, ' ');
    	pool.view( );

    	// Allocate 10 integers 3 times to test allocation on begining of pool
    	for ( int i = 0; i < 3; i++ ) {
        	allocs[i] = new( pool ) int[10];
        	std::cout << "\n[" << i << "] <<< Allocating integers\n" << string(8, ' ');
        	pool.view( );
    	}

    	// Unit tests (testing assignment)
    	for ( int i = 0; i < 10; i++ ) {
        	allocs[0][i] = i;
        	allocs[1][i] = i;
        	allocs[2][i] = i;
        	assert( allocs[0][i] == i );
        	assert( allocs[1][i] == i );
        	assert( allocs[2][i] == i );
   		}

    	// Free the first allocation (no merge)
    	delete allocs[0];
    	std::cout << "\n[0] <<< Free allocation:\n" << string(8, ' ');
    	pool.view( );

    	// Free the third allocation (merge on right side)
    	delete allocs[2];
    	std::cout << "\n[2] <<< Free allocation:\n" << string(8, ' ');
    	pool.view( );

    	// Allocate 30 integers (allocation after another element and empty spaces)
    	allocs[3] = new(pool) int[30];
    	std::cout << "\n[3] <<< Allocate 30 integers:\n" << string(8, ' ');
    	pool.view( );

    	// Unit tests (testing assignment)
    	for ( int i = 0; i < 30; i++ ) {
        	allocs[3][i] = i;
        	assert( allocs[3][i] == i );
    	}

    	// Free the second allocation (merge on left side)
    	delete allocs[1];
    	std::cout << "\n[1] <<< Free allocation:\n" << string(8, ' ');
    	pool.view( );

    	// Allocate 1 integer 2 times (at the beginning of the pool)
    	for ( int i = 4; i < 6; i++ ) {
        	allocs[i] = new(pool) int[1];
        	std::cout << "\n[" << i << "] <<< Allocate 1 integer:\n" << string(8, ' ');
        	pool.view( );
    	}

    	// Unit tests (testing assignment)
    	for ( int i = 0; i < 1; i++ ) {
        	allocs[4][i] = i;
        	allocs[5][i] = i;
        	assert( allocs[4][i] == i );
        	assert( allocs[5][i] == i) ;
    	}

    	// Free allocs[4] (no merge)
    	delete allocs[4];
    	std::cout << "\n[4] <<< Free allocation:\n" << string(8, ' ');
    	pool.view( );

    	// Free allocs[3] (merge on both sides)
    	delete allocs[3];
    	std::cout << "\n[3] <<< Free allocation:\n" << string(8, ' ');
    	pool.view( );

    	// Allocate 65 integers (fitting all the right area on pool)
    	allocs[6] = new(pool) int[65];
    	std::cout << "\n[6] <<< Allocate 65 integers:\n" << string(8, ' ');
    	pool.view( );

    	// Unit tests (testing assignment)
    	for (int i = 0; i < 65; i++) {
        	allocs[6][i] = i;
        	assert( allocs[6][i] == i );
    	}

    	// Free allocs[6] (no merge)
    	delete allocs[6];
    	std::cout << "\n[6] <<< Free allocation:\n" << string(8, ' ');
    	pool.view();

    	// Free allocs[5] (merge on both sides)
    	delete allocs[5];
    	std::cout << "\n[5] <<< Free allocation:\n" << string(8, ' ');
    	pool.view( );

    	delete pool;
	}*/
/*}}}*/

/*------------------------------ Time Counting------------------------------*/ 
/*Time Testing{{{*/
{
	std::cout << "\e[34;1m>>> Using a exaustive testing with memory pool.\e[0m\n";
    // Creates the pool
    StoragePool *pool = new SLPool(2048);

    // The initial time
    auto s = std::chrono::steady_clock::now( );
    
    // Run the function to tests
    StoragePoolTest(pool, 1);
    
    // The final time
    auto e = std::chrono::steady_clock::now( );
    
    // Calculates the difference
    auto diff = std::chrono::duration<double, std::nano>(e - s).count( );

    std::cout << ">>> Time spent: " << diff << " ns\n\n";

    // Deletes the pool
    delete pool;
}

{
	std::chrono::steady_clock::time_point start, end;
    auto time_spent = 0.0l;
    int times = 100000, *al1 = nullptr, *al2 = nullptr;

    // Average Time with the Memory Manager
	SLPool pool(400, StoragePool::FIRST_FIT);
    for ( int i = 0; i < times; i++ ) {
        start = std::chrono::steady_clock::now( );
        for (int j = 1; j <= 10; j += 2) {
            al1 = new(pool) int[j];
            al2 = new(pool) int[j+1];
            delete[] al1;
            delete[] al2;
        }

        // The final time
        end = std::chrono::steady_clock::now();
        // Calculates the difference
        auto diff = std::chrono::duration<double, std::nano>(end-start).count( );
        // Calculates the average time using standard deviation
        time_spent += (diff - time_spent)/(i+1);
    }
    std::cout << ">>> Memory Manager time with First-Fit: " << time_spent << " ns\n";

    // Average Time with the Memory Manager with Best-Fit
	SLPool pool1(400, StoragePool::BEST_FIT);
	time_spent = 0;
    for ( int i = 0; i < times; i++ ) {
        start = std::chrono::steady_clock::now( );
        for (int j = 1; j <= 10; j += 2) {
            al1 = new(pool1) int[j];
            al2 = new(pool1) int[j+1];
            delete[] al1;
            delete[] al2;
        }

        // The final time
        end = std::chrono::steady_clock::now();
        // Calculates the difference
        auto diff = std::chrono::duration<double, std::nano>(end-start).count( );
        // Calculates the average time using standard deviation
        time_spent += (diff - time_spent)/(i+1);
    }
	std::cout << ">>> Memory Manager time with Best-Fit: " << time_spent << " ns\n";

    // Average Time with the Operational System
    time_spent = 0;
    for ( int i = 0; i < times; i++ ) {
        start = std::chrono::steady_clock::now();
        for ( int j = 1; j <= 10; j += 2 ) {
            al1 = new int[j];
            al2 = new int[j+1];
            delete[] al1;
            delete[] al2;
        }
        // The final time
        end = std::chrono::steady_clock::now();
        // Calculates the difference
        auto diff = std::chrono::duration<double, std::nano>(end-start).count();
        // Calculates the average time using standard deviation
        time_spent += (diff - time_spent)/(i+1);
    }
    std::cout << ">>> Operational System time: " << time_spent << " ns\n";
}
/*}}}*/

	std::cout << "\n>>> Testing data maintenance.";

/*Testing if values are passed correctly{{{*/
{
	gm::SLPool p(220);

	int *ptr = new (p) int[8];

	for( int i=0; i < 8; i++ ){
		ptr[i] = (i+1)*5;
	}

	int *ptr1 = new int[8];

	for( int i=0; i < 8; i++ ){
		ptr1[i] = (i+1)*5;
	}

	for( int i=0; i < 8; i++ ) {
		assert( ptr[i] == ptr1[i] );
	}
	std::cout << "\n";

	delete[] ptr;
	delete[] ptr1;

	std::cout << "\e[32;1m>Data not corrupted. Maintenance successful.\n\n";
}
/*}}}*/

	std::cout << "\e[34;1m>>> Starting Tests of free operations within"
			  << " a pool.\e[0m\n";

	std::cout << "  Caso 1: Liberar uma área entre áreas livres.\n"
			  << "  Caso 2: Liberar uma área antecedendo uma área ocupada,"
			  << " e sucedendo uma área livre.\n"
			  << "  Caso 3: Liberar uma área antecedendo uma área livre,"
			  << " e sucecendo uma área ocupada.\n"
			  << "  Caso 4: Liberar uma área entre áreas ocupadas.\n"
			  << "  Caso 5: Liberar uma área antecedendo o sentinela,"
			  << " e sucedendo uma área livre.\n"
			  << "  Caso 6: Liberar uma área antecedendo o sentinela,"
			  << " e sucedendo uma área ocupada.\n\n";

	std::cout << "\t  \e[31;1m>>OBSERVATION\e[0m: Graphical presentation of"
			  << " testing cases works as:\n"
			  << "\t\tFirst 2 lines from before specified free operation.\n"
			  << "\t\tLast 2 lines from after specified free operation.\n";

/*Caso 1{{{*/
{
	std::cout << "\e[36;3mTesting Case 1:\e[0m\n";
	SLPool p(115);					// Pool with 8 blocks and 1 sentinel.
	int *ptr_a = new (p) int[5];	// Asking for 2 blocks. 5*4+8=28.
	int *ptr_b = new (p) int[8];	// Asking for 3 blocks. 8*4+8=40.
	int *ptr_c = new (p) int[5];	// Asking for 2 blocks. 5*4+8=28.

	delete[] ptr_a;
	delete[] ptr_c;
	p.view();
	// Now, ptr_b is between 2 free areas.

	delete[] ptr_b;
	p.view();
}
/*}}}*/
/*Caso 2{{{*/
{
	std::cout << "\e[36;3mTesting Case 2:\e[0m\n";
	SLPool p(115);					// Pool with 8 blocks and 1 sentinel.
	int *ptr_a = new (p) int[5];	// Asking for 2 blocks. 5*4+8=28.
	int *ptr_b = new (p) int[8];	// Asking for 3 blocks. 8*4+8=40.
	int *ptr_c = new (p) int[5];	// Asking for 2 blocks. 5*4+8=28.

	delete[] ptr_a;
	p.view();
	// Now, ptr_b is before a occupied area, and after a free one.

	delete[] ptr_b;
	p.view();
	delete[] ptr_c;					// Freeing unused memory.
}
/*}}}*/
/*Caso 3{{{*/
{
	std::cout << "\e[36;3mTesting Case 3:\e[0m\n";
	SLPool p(115);					// Pool with 8 blocks and 1 sentinel.
	int *ptr_a = new (p) int[5];	// Asking for 2 blocks. 5*4+8=28.
	int *ptr_b = new (p) int[8];	// Asking for 3 blocks. 8*4+8=40.
	int *ptr_c = new (p) int[5];	// Asking for 2 blocks. 5*4+8=28.

	delete[] ptr_c;
	p.view();
	// Now, ptr_b is before a free area, and after a occupied one.

	delete[] ptr_b;
	p.view();
	delete[] ptr_a;					// Freeing unused memory.
}
/*}}}*/
/*Caso 4{{{*/
{
	std::cout << "\e[36;3mTesting Case 4:\e[0m\n";
	SLPool p(115);					// Pool with 8 blocks and 1 sentinel.
	int *ptr_a = new (p) int[5];	// Asking for 2 blocks. 5*4+8=28.
	int *ptr_b = new (p) int[8];	// Asking for 3 blocks. 8*4+8=40.
	int *ptr_c = new (p) int[5];	// Asking for 2 blocks. 5*4+8=28.

	p.view();
	// Now, ptr_b is between 2 occupied areas.

	delete[] ptr_b;
	p.view();
	delete[] ptr_a;					// Freeing unused memory.
	delete[] ptr_c;					// Freeing unused memory.
}
/*}}}*/
/*Caso 5{{{*/
{
	std::cout << "\e[36;3mTesting Case 5:\e[0m\n";
	SLPool p(115);					// Pool with 8 blocks and 1 sentinel.
	int *ptr_a = new (p) int[8];	// Asking for 3 blocks. 5*4+8=28.
	int *ptr_b = new (p) int[8];	// Asking for 3 blocks. 8*4+8=40.
	int *ptr_c = new (p) int[5];	// Asking for 2 blocks. 5*4+8=28.

	delete[] ptr_b;
	p.view();
	// Now, ptr_c is before the sentinel, and after a free area.
	
	delete ptr_c;
	p.view();
	delete[] ptr_a;					// Freeing unused memory.

}
/*}}}*/
/*Caso 6{{{*/
{
	std::cout << "\e[36;3mTesting Case 6:\e[0m\n";
	SLPool p(115);					// Pool with 8 blocks and 1 sentinel.
	int *ptr_a = new (p) int[8];	// Asking for 3 blocks. 5*4+8=28.
	int *ptr_b = new (p) int[8];	// Asking for 3 blocks. 8*4+8=40.
	int *ptr_c = new (p) int[5];	// Asking for 2 blocks. 5*4+8=28.

	p.view();
	// Now, ptr_c is before the sentinel, and after a occupied area.
	
	delete ptr_c;
	p.view();
	delete[] ptr_a;					// Freeing unused memory.
	delete[] ptr_b;					// Freeing unused memory.

}
/*}}}*/

/*Data preservation test{{{*/
	std::cout << "\n";
{
	SLPool q(130, StoragePool::BEST_FIT);	// Pool with 9 blocks and 1 sentinel
	
	double *ptr = new (q) double[10];	// Allocates 6 blocks.
	q.view();
	double *ptr1 = new (q) double[4];	// Allocates 3 blocks.
	q.view();

	bool oziel;
	try{ int *pointer = new (q) int[2]; delete pointer; }
	catch ( std::bad_alloc& e ){
		oziel = true;
		std::cerr << "Can't allocate more space within pool!"
				  << "Create another pool or use the Operational System"
				  << "(OS) allocation functions.\n";
	}

	assert( oziel );
	delete ptr1;
	delete ptr;
}
/*}}}*/
    std::cout << "\n\e[32;4m>>> Exiting successfully...\e[0m";

	return EXIT_SUCCESS;
}
