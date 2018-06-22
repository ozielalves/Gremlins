/**
 * @file storage_pool.hpp
 * @version 1.0
 * @since Jun, 13. 
 * @date Jun, 22.
 * @author Oziel Alves (ozielalves@ufrn.edu.br)
 * @author Daniel Guerra (daniel.guerra13@hotmail.com)
 * @title Class sc::StoragePool
 */

/* "Para a implementação deste projeto é obrigatório a 
	implementação da sua própria lista encadeada." 	*/

# ifndef _STORAGE_POOL_HPP_
# define _STORAGE_POOL_HPP_

#include <iostream>
//#include <vector>

/**
 * @brief StoragePool class's declaration
 */

namespace sc
{
	typedef T size_type;
	
	template< typename T >
	class StoragePool{
		
	//private:

	public:
    	/**
    	 * @brief StoragePool destructor
    	 */
    	virtual ~StoragePool( ) { /*Empty*/ }

    	/**
    	 * @brief Allocates memory
	     * @param _b Number of bytes to be allocated
	     * @return A pointer to the beggining of the allocated area
    	 */
	    virtual void *Allocate( size_type _b ) /*= 0*/;

	    /**
	     * @brief Free memory
	     * @param _p A pointer to element to be freed
	     */
	    virtual void Free(void *_p) /*= 0*/;
	}

}

#endif
