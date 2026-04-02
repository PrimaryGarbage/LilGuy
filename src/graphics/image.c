#include <stdlib.h>
#include "image.h"

void Image_Free(Image* image)
{
    free(image->data);
}
