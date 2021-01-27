 #ifndef DISPLAYLISTS_H
 #define DISPLAYLISTS_H

 #include <GL/freeglut.h>

class DisplayLists
{
public:
	DisplayLists(int size=1)
		: size_(size), allocated_(false)
	{
		allocate(size);
	}
	
	
	void allocate(int size)
	{
		if (allocated_)
		{
			deallocate();
		}
		size_ = size;
		id_ = glGenLists(size);
		allocated_ = true;
	}
	
	
	void deallocate()
	{
		glDeleteLists(id_, size_);
		allocated_ = false;
	}
	
	
	void begin(int i=0)
	{
		glNewList(id_ + i, GL_COMPILE);
	}
	
	
	void end()
	{
		glEndList();
	}

	
	void draw(int i = 0)
	{
		glCallList(id_ + i);
	}

	
	void draw(GLubyte indices[], int size)
	{
		glListBase(id_);
		glCallLists(size, GL_UNSIGNED_BYTE, indices);
	}


private:
	GLsizei size_;
	GLuint id_;
	bool allocated_;
};

#endif
