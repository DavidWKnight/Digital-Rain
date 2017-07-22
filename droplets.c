#include "droplets.h"

static const char max_char = 126, min_char = 33;
static unsigned int max_row, max_col;
static unsigned int num_of_droplets;
static struct droplet *droplets;

int init_droplets(){
    srand( time(NULL) );
    getmaxyx(stdscr, max_row, max_col);
    unsigned int screen_area = max_row*max_col;

    num_of_droplets = screen_area/50;
    droplets = malloc(sizeof(struct droplet) * num_of_droplets);

    if(!droplets){
        printf("Out of memory\n");
        exit(1);
    }

    unsigned int i = 0;
    for (; i < num_of_droplets; i++){
        gen_droplet_props(droplets + i);
        (droplets + i)->row = rand() % max_row;
    }

    return 0;
}

void cleanup(){
    free(droplets);
    endwin();
    return;
}

int draw_rain(){
    //check for change in window size
    unsigned int max_row_temp = max_row, max_col_temp = max_col;
    getmaxyx(stdscr, max_row, max_col);
    if(max_col_temp != max_col || max_row_temp != max_row){
        clear();
        if(droplets_realloc(max_col * max_row / 50)){
            printf("realloc error\n");
            exit(1);
        }
    }
    
    struct droplet *i = droplets;
    for(; i < droplets+num_of_droplets; i++){
        draw_droplet(i);
        
        //check if droplet is entirely offscreen
        if((i)->row > max_row + (i)->length){
            mvaddch((i)->row-(i)->length-2, (i)->col, ' ');
            gen_droplet_props(i);
        }
    }
    return 0;
}

static int draw_droplet(struct droplet *drop){
    int temp;
    if(velocity){
        temp = drop->length/2 - 2 + refresh_rate;
    }
    else{
        temp = refresh_rate;
    }

    if(drop->frames_per_row % (temp/2+1)){
        drop->frames_per_row++;
        return 0;
    }

    if(drop->row < 1){
        drop->frames_per_row = 1;
        drop->row++;
        return 0;
    }

    mvaddch(drop->row-drop->length-2, drop->col, ' ');

    char color;
    
    #ifdef TASTE_THE_RAINBOW
    if((max_row >> 3)-2 < 1){
        color = 2;
    }
    else{
        color = 2+5*((float)drop->length-2)/((max_row >> 2)-2);
    }
    if (color > 8){
        color = 8;
    }
    else if(color < 2){
        color = 0;
    }
    #else //TASTE_THE_RAINBOW
    if(!multicolor_trail){
        color = primary_color;
    }
    else if(multicolor_trail == 2){
        color = secondary_color;
    }
    else{
        if(rand()%2){
            color = primary_color;
        }
        else{
            color = secondary_color;
        }
    }
    #endif //TASTE_THE_RAINBOW
    
    attron(COLOR_PAIR(color));
    mvaddch(drop->row-1, drop->col, rand() % (max_char - min_char) + min_char);
    attroff(COLOR_PAIR(color));
    
    attron(COLOR_PAIR(8));
    mvaddch(drop->row, drop->col, rand() % (max_char - min_char) + min_char); 
    attroff(COLOR_PAIR(8));

    drop->frames_per_row = 1;
    drop->row++;

    return 0;
}

static int gen_droplet_props(struct droplet *drop){
    drop->row = -(rand() % max_row);
    drop->col = rand() % max_col;
    drop->length = (rand() % (max_row >> 2)) + 2;
    drop->frames_per_row = 0;
    return 0;
}

static int droplets_realloc(unsigned int new_num_of_droplets){
    unsigned int temp;
    struct droplet *temp_droplets;

    //determine how many droplets get copied over from old pointer
    if(new_num_of_droplets == num_of_droplets){
        return 0;
    }
    if(new_num_of_droplets > num_of_droplets){
        temp = num_of_droplets;//new alloc is bigger than current
    }
    else{
        temp = new_num_of_droplets;//new alloc is smaller than current
    }

    temp_droplets = malloc(sizeof(struct droplet) * new_num_of_droplets);

    if(!temp_droplets){
        return 1;
    }
    
    memcpy(temp_droplets, droplets, temp*sizeof(struct droplet));

    struct droplet *x;
    x = temp_droplets;
    temp_droplets = droplets;
    droplets = x;
    
    free(temp_droplets);

    num_of_droplets = new_num_of_droplets;

    //initialize all new droplets
    for(; temp < num_of_droplets; temp++){
        gen_droplet_props(droplets + temp);
        (droplets + temp)->row = rand() % max_row;
    }

    return 0;
}
