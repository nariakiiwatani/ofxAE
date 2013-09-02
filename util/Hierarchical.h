#pragma once

#include "stddef.h"

class Hierarchical
{
public:
	Hierarchical():parent_(NULL),child_(NULL),sibling_(NULL),dirty_flags_(NONE){}
	
	void setParent(Hierarchical* p);
	Hierarchical* getParent() { return parent_; }
	Hierarchical* getFirstChild() { return child_; }
	Hierarchical* getSibling() { return sibling_; }

	bool isDirty(unsigned int chk=0xFFFFFFFF) const{return (dirty_flags_&chk)!=0;}

protected:
	Hierarchical* parent_;
	Hierarchical* child_;
	Hierarchical* sibling_;
	enum {
		NONE	= 0x00000000,
		LOCAL	= 0x00000001,
		PARENT	= 0x00000002,
	};
	void dirty(unsigned int flag);
	void clsDirtyFlag(unsigned int flag){dirty_flags_&=~flag;}
private:
	unsigned int dirty_flags_;
};

/* EOF */
