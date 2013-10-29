#ifndef MEMORY_ARENA
#define MEMORY_ARENA

/**
* \class MemoryArena
* \brief Manager over a memory pool
* \author Felix-Antoine Ouellet
*/
class MemoryArena
{
private:
	void* m_Memory;					/*!< Pointer to the chunk of managed memory */
	unsigned long m_ChunkSize;		/*!< Size of managed memory */

public:
	/**
	* \fn MemoryArena
	* \brief Default constructor
	* \param in_ChunkSize Size of the memory chunk to allocate
	*/
	MemoryArena(unsigned long in_ChunkSize = 0);
	
	/**
	* \fn ~MemoryArena
	* \brief Destructor
	*/
	~MemoryArena();

	// Q : Good design ??
	/**
	* \fn GetPtr
	* \brief Accessor to the memory pointer
	*/
	void* GetPtr();

	// TODO : Memory management

	// Q : How involved should it be in the overwriting of cold blocks ??
};

#endif // MEMORY_ARENA
