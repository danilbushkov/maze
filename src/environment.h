#ifndef ENV_H
#define ENV_H


class Environment{
    public:
        Environment();
        void show();
        int getCountStates();

    private:
        int field[100];
        int agentPosition;
        int countStates;

        void initField();

    

};

#endif