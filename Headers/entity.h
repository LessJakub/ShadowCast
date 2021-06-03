#ifndef ENTITY_H
#define ENTITY_H


class Entity
{
public:
    Entity() :id(counter++) {}

    //Id of each entitiy created, used for key values in maps
    int id;

    //Used for incrementation of id with each creation of element
    static int counter;
};

#endif // ENTITY_H
