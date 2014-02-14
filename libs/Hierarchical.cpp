#include "Hierarchical.h"

void Hierarchical::setParent(Hierarchical* p)
{
	if(parent_) {
		if(this == parent_->child_) {
			parent_->child_ = sibling_;
		}
		else {
			Hierarchical* c = parent_->child_;
			while(c) {
				if(c->sibling_ == this) {
					c->sibling_ = sibling_;
					break;
				}
				c = c->sibling_;
			}
		}
		sibling_ = NULL;
	}
	if(p) {
		sibling_ = p->child_;
		p->child_ = this;
	}
	parent_ = p;
	dirty(PARENT);
}

void Hierarchical::dirty(unsigned int flag)
{
	dirty_flags_ |= flag;
	if(child_) {
		child_->dirty(PARENT);
	}
	if(sibling_ && flag & PARENT) {
		sibling_->dirty(flag);
	}
}


/* EOF */