#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/**
 * Grow and multiply your organisms to end up larger than your opponent.
 **/

typedef struct s_ent_bio
{
    // grid coordinate
    int x;
    int y;
    // WALL, ROOT, BASIC, TENTACLE, HARVESTER, SPORER, A, B, C, D
    char type[33];
    // 1 if your organ, 0 if enemy organ, -1 if neither
    int owner;
    // id of this entity if it's an organ, 0 otherwise
    int organ_id;
    // N,E,S,W or X if not an organ
    char organ_dir[2];
    int organ_parent_id;
    int organ_root_id;
}   ent_bio;

typedef struct s_organ_bio
{
    int     *coor;
    char    *type;
    char    dir[2];
}   organ_bio;

int aim_coor[2];

int seek_own_ent(ent_bio **ent_lib, int i)
{
    while (ent_lib[i]->owner != 1)
        i++;
    return (i);
}

int *check_cell(ent_bio **ent_lib, int x, int y)
{
    int *coor = malloc(2 * sizeof(int));
    int i = 0;

    while (ent_lib[i] && (ent_lib[i]->x != x || ent_lib[i]->y != y))
        i++;
        
    if(!ent_lib[i])
    {
        coor[0] = x;
        coor[1] = y;
        return (coor);
    }
    else
        return (NULL);
}

int check_protein(ent_bio **ent_lib, int x, int y, char *prot)
{
    int i = 0;

    while (ent_lib[i] && (ent_lib[i]->x != x || ent_lib[i]->y != y))
        i++;

    if (ent_lib[i] && !(strcmp(ent_lib[i]->type, prot)))
        return (1);
    return (0);
}

int check_enemy(ent_bio **ent_lib, int x, int y)
{
    int i = 0;

    while (ent_lib[i] && (ent_lib[i]->x != x || ent_lib[i]->y != y))
        i++;

    if (ent_lib[i] && !ent_lib[i]->owner)
        return (1);
    return (0);
}

int check_proteins(ent_bio **ent_lib, organ_bio **new_organ_p, int *coor, int my_c, int my_d)
{
    int flag = check_protein(ent_lib, coor[0], coor[1] - 1, "A");
    (*new_organ_p)->dir[0] = 'N';
    (*new_organ_p)->dir[1] = '\0';
    if (!flag)
    {
        flag = check_protein(ent_lib, coor[0], coor[1] + 1, "A");
        (*new_organ_p)->dir[0] = 'S';
        (*new_organ_p)->dir[1] = '\0';
    }
    if (!flag)
    {
        flag = check_protein(ent_lib, coor[0] + 1, coor[1], "A");
        (*new_organ_p)->dir[0] = 'E';
        (*new_organ_p)->dir[1] = '\0';
    }
    if (!flag)
    {
        flag = check_protein(ent_lib, coor[0] - 1, coor[1], "A");
        (*new_organ_p)->dir[0] = 'W';
        (*new_organ_p)->dir[1] = '\0';
    }
    if (flag && my_c && my_d)
    {
        (*new_organ_p)->type = malloc(strlen("HARVESTER") + 1 * sizeof(char));
        (*new_organ_p)->type = "HARVESTER";
    }
    return (flag);
}

void check_enemies(ent_bio **ent_lib, organ_bio **new_organ_p, int *coor, int my_b, int my_c)
{
    int flag = check_enemy(ent_lib, coor[0], coor[1] - 1);
    (*new_organ_p)->dir[0] = 'N';
    (*new_organ_p)->dir[1] = '\0';
    if (!flag)
    {
        flag = check_enemy(ent_lib, coor[0], coor[1] + 1);
        (*new_organ_p)->dir[0] = 'S';
        (*new_organ_p)->dir[1] = '\0';
    }
    if (!flag)
    {
        flag = check_enemy(ent_lib, coor[0] + 1, coor[1]);
        (*new_organ_p)->dir[0] = 'E';
        (*new_organ_p)->dir[1] = '\0';
    }
    if (!flag)
    {
        flag = check_enemy(ent_lib, coor[0] - 1, coor[1]);
        (*new_organ_p)->dir[0] = 'W';
        (*new_organ_p)->dir[1] = '\0';
    }
    if (flag && my_b && my_c)
    {
        (*new_organ_p)->type = malloc(strlen("TENTACLE") + 1 * sizeof(char));
        (*new_organ_p)->type = "TENTACLE";
    }
    else
    {
        (*new_organ_p)->type = malloc(strlen("BASIC") + 1 * sizeof(char));
        (*new_organ_p)->type = "BASIC";
    }
}

int *look_for_prot_source(ent_bio **ent_lib, organ_bio **new_organ_p, int *coor, int width, int height)
{
    int *aim_coor = malloc(2 * sizeof(int));
    int dir[2] = {0, 0};
    int i = 0;

    for (int t = 4; t > 0; t--)
    {
        aim_coor[0] = coor[0];
        aim_coor[1] = coor[1];
        i = 0;
        switch (t) 
        {
            case 4:
                dir[0] = -1;
                break;
            case 3:
                dir[0] = 1;
                break;
            case 2:
                dir[0] = 0;
                dir[1] = -1;
                break;
            default:
                dir[1] = 1;
                break;
        }

        while (aim_coor[0] < height && aim_coor[1] < width && aim_coor[0] >= 0 && aim_coor[1] >= 0)
        {
            while (ent_lib[i] && (ent_lib[i]->x != aim_coor[1] || ent_lib[i]->y != aim_coor[0]))
                i++;
            if (ent_lib[i]) break ;
            aim_coor[0] += dir[0];
            aim_coor[1] += dir[1];
            i = 0;
        }

        if (ent_lib[i] && (!strcmp(ent_lib[i]->type, "A") || !strcmp(ent_lib[i]->type, "B") || !strcmp(ent_lib[i]->type, "C") || !strcmp(ent_lib[i]->type, "D")))
        {
            (*new_organ_p)->type = malloc(strlen("SPORER") + 1 * sizeof(char));
            (*new_organ_p)->type = "SPORER";
            aim_coor[0] -= dir[0] * 2;
            aim_coor[1] -= dir[1] * 2;
            if (dir[0] == -1)
            {
                (*new_organ_p)->dir[0] = 'N';
                (*new_organ_p)->dir[1] = '\0';
            }
            if (dir[0] == 1)
            {
                (*new_organ_p)->dir[0] = 'S';
                (*new_organ_p)->dir[1] = '\0';
            }
            if (dir[1] == -1)
            {
                (*new_organ_p)->dir[0] = 'W';
                (*new_organ_p)->dir[1] = '\0';
            }
            else
            {
                (*new_organ_p)->dir[0] = 'E';
                (*new_organ_p)->dir[1] = '\0';
            }
            return (aim_coor);
        }
    }

    return (NULL);
}

int *check_cells(ent_bio **ent_lib, organ_bio **new_organ_p, int next_own_ent, int my_b, int my_c, int my_d, int width, int height)
{
    int     *coor = NULL;
    int     dir[2] = {0, 0};
        
    coor = check_cell(ent_lib, ent_lib[next_own_ent]->x + 1, ent_lib[next_own_ent]->y);
    if (!coor)
        coor = check_cell(ent_lib, ent_lib[next_own_ent]->x, ent_lib[next_own_ent]->y - 1);
    else
        dir[0] = -1;
    if (!coor)
        coor = check_cell(ent_lib, ent_lib[next_own_ent]->x, ent_lib[next_own_ent]->y + 1);
    else
        dir[0] = 1;
    if (!coor)
    {
        coor = check_cell(ent_lib, ent_lib[next_own_ent]->x - 1, ent_lib[next_own_ent]->y);
        dir[1] = 1;
    }
    else
        dir[1] = -1;
    
    if(coor)
    {
        if (my_b && my_d)
        {
            int *aux_aim_coor = look_for_prot_source(ent_lib, new_organ_p, coor, width, height);
            if (aux_aim_coor)
            {
                aim_coor[0] = aux_aim_coor[1];
                aim_coor[1] = aux_aim_coor[0];
                return (coor);
            }
        }
        if (check_proteins(ent_lib, new_organ_p, coor, my_c, my_d)) return (coor);
        check_enemies(ent_lib, new_organ_p, coor, my_b, my_c);
    }

    return (coor);
}

int look_for_sporer(ent_bio **ent_lib)
{
    int i;
    for (i = 0; ent_lib[i]; i++)
        if (!(strcmp(ent_lib[i]->type, "SPORER")) && ent_lib[i]->owner == 1) return (i);
    return (0);
}

int look_for_second_root(ent_bio **ent_lib)
{
    int i;
    for (i = 0; ent_lib[i]; i++)
        if (!(strcmp(ent_lib[i]->type, "ROOT")) && ent_lib[i]->owner == 1 && ent_lib[i]->organ_id > 2) return (i);
    return (0);
}

int main()
{
    // columns in the game grid
    int width;
    // rows in the game grid
    int height;
    scanf("%d%d", &width, &height);

    ent_bio **ent_lib = NULL;

    // game loop
    while (1) 
    {
        int entity_count;
        scanf("%d", &entity_count);

        ent_lib = realloc(ent_lib, entity_count * sizeof(ent_bio*) + 1);
        if (!ent_lib) 
        {
            perror("Error al asignar memoria para ent_bio");
            return 1;
        }

        for (int i = 0; i < entity_count; i++) 
        {
            if (!ent_lib[i]) 
            {
                ent_lib[i] = malloc(sizeof(ent_bio));
                if (!ent_lib[i]) 
                {
                    perror("Error al asignar memoria para ent_bio");
                    return 1;
                }
            }
            scanf("%d%d%s%d%d%s%d%d", &(ent_lib[i]->x), &(ent_lib[i]->y), ent_lib[i]->type, 
                &(ent_lib[i]->owner), &(ent_lib[i]->organ_id), ent_lib[i]->organ_dir, 
                &(ent_lib[i]->organ_parent_id), &(ent_lib[i]->organ_root_id));
        }
        ent_lib[entity_count] = NULL;

        // your protein stock
        int my_a;
        int my_b;
        int my_c;
        int my_d;
        scanf("%d%d%d%d", &my_a, &my_b, &my_c, &my_d);

        // opponent's protein stock
        int opp_a;
        int opp_b;
        int opp_c;
        int opp_d;
        scanf("%d%d%d%d", &opp_a, &opp_b, &opp_c, &opp_d);

        // your number of organisms, output an action for each one in any order
        int required_actions_count;
        int next_own_ent = 0;
        scanf("%d", &required_actions_count);
        for (int i = 0; i < required_actions_count; i++) 
        {
            // Write an action using printf(). DON'T FORGET THE TRAILING \n
            // To debug: fprintf(stderr, "Debug messages...\n");
            organ_bio *new_organ = malloc(sizeof(organ_bio*));
            if (!new_organ) 
            {
                perror("Error al asignar memoria para new_organ");
                return 1;
            }
            next_own_ent = seek_own_ent(ent_lib, next_own_ent);

            new_organ->coor = NULL;
            new_organ->coor = check_cells(ent_lib, &new_organ, next_own_ent, my_b, my_c, my_d, width, height);
            while (!new_organ->coor && next_own_ent <= entity_count)
            {
                next_own_ent = seek_own_ent(ent_lib, ++next_own_ent);
                new_organ->coor = check_cells(ent_lib, &new_organ, next_own_ent, my_b, my_c, my_d, width, height);
            }
            fprintf(stderr, "%d\n", required_actions_count);
            if (my_a && my_b && my_c && my_d)
            {
                int sporer_ind = look_for_sporer(ent_lib);
                if (sporer_ind)
                    printf("SPORE %d %d %d\n", ent_lib[sporer_ind]->organ_id, aim_coor[0], aim_coor[1]);
            }

            if (required_actions_count == 2)
                next_own_ent = look_for_second_root(ent_lib);
            if (!(strcmp(new_organ->type, "SPORER")))
                printf("GROW %d %d %d SPORER %c\n", ent_lib[next_own_ent]->organ_id, new_organ->coor[0], new_organ->coor[1], new_organ->dir[0]);
            else if (!(strcmp(new_organ->type, "HARVESTER")))
                printf("GROW %d %d %d HARVESTER %c\n", ent_lib[next_own_ent]->organ_id, new_organ->coor[0], new_organ->coor[1], new_organ->dir[0]);
            else if (!(strcmp(new_organ->type, "TENTACLE")))
                printf("GROW %d %d %d TENTACLE %c\n", ent_lib[next_own_ent]->organ_id, new_organ->coor[0], new_organ->coor[1], new_organ->dir[0]);
            else
                printf("GROW %d %d %d BASIC\n", ent_lib[next_own_ent]->organ_id, new_organ->coor[0], new_organ->coor[1]);
        }
        fprintf(stderr, "\n");
    }

    return 0;
}