#include "interface_image_source.h"

void SourceImageDisplayInterface::passToLink() { if(LinkNode != nullptr) LinkNode->setImageFrom(this); }