#include <iostream>
#include <fstream>
#include <unistd.h>
#include "actions.h"
#include "agent.h"
#include <ctime>
#include <cstdlib>
#include <cmath>




actions Agent::move(){
    actions direction = up;
    for(int i=1; i<4; ++i){
        if(qtable[state][direction]<qtable[state][i]){
            direction = static_cast<actions>(i);
        }
    }
    action = direction;
    return direction;
}

actions Agent::moveSoftmax(){
    
    int n = 1 + rand() % 100;
    int p = 0;
    actions direction = up;
    for(int i=0; i<4; ++i){
        p = p + round(softmax(qtable[state][i])*100);
        if(p>=n){
            direction = static_cast<actions>(i);
            break;
        }
    }
    action = direction;
    return direction;
}

float Agent::softmax(float u){
    return exp(u/K)/(exp(qtable[state][0]/K)+
                   exp(qtable[state][1]/K)+
                   exp(qtable[state][2]/K)+
                   exp(qtable[state][3]/K));
}

void Agent::setK(float k){
    K = k;
}



void Agent::analysis(moveResult m){
    
    if((m.reward > 0) || (m.reward < 0)){
        qtable[state][action] = qtable[state][action] + m.reward;
        
    }else{
        qtable[state][action] = qtable[state][action] +
            LF*(m.reward+DF*max(m.state)-qtable[state][action]);
    }
    state = m.state;
    
}

float Agent::max(int state){
    float r = qtable[state][up];
    for(int i = 1; i<4; ++i){
        if(qtable[state][i]>r){
            r = qtable[state][i];
        }
    }
    return r;
}

Agent::Agent(int countStates, int state){
    srand(time(NULL));
    this->countStates = countStates;
    this->state = state;

    qtable = new float*[countStates];
    for(int i = 0; i < countStates; ++i){
        qtable[i] = new float[4];
    }
    InitQTable();
}



Agent::~Agent(){
    

    for(int i = 0; i<countStates; i++){
        delete [] qtable[i];
    }
    delete [] qtable;
}


void Agent::saveQTable(){
    std::ofstream out("./qtable.txt");
    
    if(out.is_open()){
        for(int i = 0; i<countStates; i++){
            
            for(int k = 0; k<4; ++k){
                out << qtable[i][k] << ' ';
            }
            out << std::endl;
        }
        
    }
    out.close();
}

void Agent::InitQTable(){

    std::ifstream in("./qtable.txt");

    if(in.is_open()){
        float k;

        int i = 0;
        int m = 0;
        while(in >> k){
            if(i<countStates){
                qtable[i][m]=k;
            }
            if(m == 3){
                m = 0;
                ++i;
            }else{
                ++m;
            }
            
        }
    }

    in.close();
}