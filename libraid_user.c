#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DEVICES 100
#define MAXSTR 200


struct system
{
    int num_raids;
    struct raid *raids;
};

struct raid
{
    int raid_id;
    char raid_name[MAXSTR];
    int stripe_size;
    int bps;                    // blocks per sector
    int num_devices;
    struct device *devices[MAX_DEVICES];
    int raidsize;
    struct raid * next;
};

struct device
{
    char *name;
    int  size; 
};


int create_new_raid(struct system * s, char *raid_name, int num_devices, ...)
{
    va_list list;
    int i = 0;
    struct raid *r;
    struct device *disk;

    // initialize one raid node
    r = malloc(sizeof(struct raid));
    if (r != NULL)
    {
        memset(r, 0, sizeof(struct raid));
    }

    r->next = NULL;

    // set raid id
    r->raid_id=s->num_raids;

    // set raid name 
    strcpy(r->raid_name,raid_name);

    // set number of devices
    r->num_devices = num_devices;

    // iterate over each device and set name 
    va_start(list, num_devices);
    for(i=0; i<num_devices; i++)
    {
        // initialize disk
        disk = malloc(sizeof(struct device));
        memset(disk, 0, sizeof(struct device));
        // set each name using va_arg(list, char *)
        disk->name = va_arg(list, char *);
        r->devices[i] = disk;
    }
    va_end(list);

    // TODO : add raid node to linked list
    if (s->raids == NULL){
        // just add
        s->raids = r;
    } else {
        // iterate to end and then add
        while(s->raids->next != NULL)
            s->raids = s->raids->next;
        s->raids->next = r;
    }
    // s->raids[s->num_raids] = r;
    // increment system.num_raids
    s->num_raids += 1;
    return EXIT_SUCCESS;
} 


int delete_raid(struct system *s, int id)
{
    struct raid *r = s->raids;
    int i = 0;

    // free all disks
    while (s->raids->raid_id == id || s->raids != NULL)
    {
        r = s->raids;
        s->raids = s->raids->next;
    }

    if (s->raids == NULL){
        printf("Raid with ID %d does not exists.", id);
        return 0;
    }

    // free all disks
    for (i=0; i<s->raids->num_devices; i++)
    {
        free(s->raids->devices[i]);
    }
    
    // free raid
    r->next = s->raids->next;
    free(s->raids);

    s->num_raids--;
    
    return 0;
}

int initialize_system(struct system **sp)
{
    // Do not let more than one system exists together
    if (*sp != NULL)
    {
        return 0;
    }

    *sp = malloc(sizeof(struct system));

    if(*sp != NULL)
    {
        memset(*sp, 0, sizeof(struct system));
    }
    return 0;
}

void print_raid_config(struct system *s)
{
    int j=0;

    printf("\n");
    printf("Total Raids: %d\n", s->num_raids);

    if (s->raids == NULL)
    {
        return;
    }

    while(s->raids != NULL)
    {
        printf("Raid ID: %d\n", s->raids->raid_id);
        printf("Raid Name: %s\n", s->raids->raid_name);
        for (j=0; j<s->raids->num_devices; j++)
        {
            printf("device: %s\n", s->raids->devices[j]->name);
        }
        s->raids = s->raids->next;
    }

}

int main()
{
    struct system * s;
    char * rname,* lname;
    char * d0;
    char * d1;
    int id = 1;

    rname = malloc(sizeof(43));
    lname = malloc(sizeof(43));
    rname = "ankit_raid";
    lname = "rana_raid";
    d0 = malloc(sizeof(43));
    d0 = "/dev/sdb";
    d1 = malloc(sizeof(43));
    d1 = "/dev/sdc";    

    // initialize system structure
    initialize_system(&s);
    // set raid config
    create_new_raid(s, rname, 2, d0, d1);
    create_new_raid(s, lname, 2, d0, d1);
    //create_new_raid(s, rname, 0);
    // view raid config
    print_raid_config(s);
    delete_raid(s,id);
    //print_raid_config(s);

    return 0;
}

