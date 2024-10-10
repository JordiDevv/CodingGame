#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

typedef struct building_type
{
    int    type;
    int    id;
    int    x;
    int    y;
    int    n_astronauts;
    int    astronauts[20];
    int    types_astr;
    int    tube;
}   building_struct;

building_struct **buildings;
int             b_counter = 0;
int             n_pod = 0;
int             t_counter = 0;

float distance(building_struct *p1, building_struct *p2) 
{
    return sqrt((p2->x - p1->x) * (p2->x - p1->x) + (p2->y - p1->y) * (p2->y - p1->y));
}

float pointOnSegment(building_struct *A, building_struct *B, building_struct *C) 
{
    float epsilon = 0.0000001;
    return (-epsilon < distance(B, A) + distance(A, C) - distance(B, C) < epsilon);
}

int check_obstacles(int beg, int dst)
{
    for (int i = 0; i < b_counter; i++)
    {
        if (pointOnSegment(buildings[i], buildings[beg], buildings[dst]))
            return (1);
    }
    return (0);
}

int ctoi(char   c)
{
    return (c - 48);
}

int match_types(int in, int *d)
{
    while (*d < 20)
    {
        if (buildings[in]->astronauts[*d])
        {
            for (int j = 0; j < b_counter; j++)
            {
                if (buildings[j]->type == *d + 1)
                {
                    (*d)++;
                    return (j);
                }
            }
        }
        (*d)++;
    }
    return (0);
}

int parse_properties(char *building_properties, int *in)
{
    int r = 0;

    while (building_properties[*in] >= '0' && building_properties[*in] <= '9')
    {
        r *= 10;
        r += ctoi(building_properties[*in]);
        (*in)++;
    }
    (*in)++;
    return (r);
}

void    add_building(char *building_properties)
{
    int in = 0;

    buildings[b_counter] = malloc(sizeof(building_struct));
    buildings[b_counter]->type = parse_properties(building_properties, &in);
    buildings[b_counter]->id = parse_properties(building_properties, &in);
    buildings[b_counter]->x = parse_properties(building_properties, &in);
    buildings[b_counter]->y = parse_properties(building_properties, &in);
    if (buildings[b_counter]->type == 0)
    {
        buildings[b_counter]->n_astronauts = parse_properties(building_properties, &in);
        buildings[b_counter]->types_astr = 0;
        for (int i = 0; i < 20; i++)
            buildings[b_counter]->astronauts[i] = 0;
        int j = buildings[b_counter]->n_astronauts;
        while (j > 0)
        {
            int type_astronaut = parse_properties(building_properties, &in);
            buildings[b_counter]->astronauts[type_astronaut - 1]++;
            j--;
        }
        for (int i = 0; i < 20; i++)
        {
            if(buildings[b_counter]->astronauts[i])
                buildings[b_counter]->types_astr++;
        }
    }
    else
        buildings[b_counter]->n_astronauts = 0;
    buildings[b_counter]->tube = 0;
    b_counter++;
}

int main()
{
    buildings = malloc(15 * sizeof(building_struct*));
    // game loop
    while (1) 
    {
        int resources;
        scanf("%d", &resources);

        int num_travel_routes;
        scanf("%d", &num_travel_routes); fgetc(stdin);
        for (int i = 0; i < num_travel_routes; i++) 
        {
            int building_id_1;
            int building_id_2;
            int capacity;
            scanf("%d%d%d", &building_id_1, &building_id_2, &capacity);
        }

        int num_pods;
        scanf("%d", &num_pods); fgetc(stdin);
        for (int i = 0; i < num_pods; i++) 
        {
            char pod_properties[201];
            scanf("%[^\n]", pod_properties); fgetc(stdin);
        }
        n_pod = num_pods;

        int num_new_buildings;
        scanf("%d", &num_new_buildings); fgetc(stdin);
        for (int i = 0; i < num_new_buildings; i++) 
        {
            char building_properties[501];
            scanf("%[^\n]", building_properties); fgetc(stdin);
            add_building(building_properties);
        }

        for (int i = 0; i < b_counter; i++)
        {
            int j = buildings[i]->types_astr;
            int d = 0;
            int dst = 0;
            while (buildings[i]->type == 0 && j > 0)
            {
                match_types(i, &d);
                dst = d;
                if (d < i)
                    dst = d - 1;
                /*if (check_obstacles(i, dst))
                {
                    if (i + 1 >= b_counter)
                    {
                        if (!(check_obstacles(i, i - 1)))
                            dst = i - 1;
                    }
                    else if (!(check_obstacles(i, i + 1)))
                        dst = i + 1;
                }*/
                printf("TUBE %i %i; POD %i %i %i %i %i %i %i %i %i %i %i\n", \
                    i, dst, n_pod, i, dst, i, dst, i, dst, i, dst, i, dst);
                n_pod++;
                j--;
            }
        }

        // Write an action using printf(). DON'T FORGET THE TRAILING \n
        // To debug: fprintf(stderr, "Debug messages...\n");

        // TUBE | UPGRADE | TELEPORT | POD | DESTROY | WAIT
    }

    return 0;
}