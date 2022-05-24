#include <stdio.h>   // standart giris cikis islemleri  
#include "raylib.h"  
#include <stdlib.h>  // random islemler 
#include <time.h>    
#include <unistd.h>  // sleep 

typedef struct Snake{
    int width ;
    int height;
    char *color;
    int position_x;
    int position_y;
    int step;
    KeyboardKey last_key_pressed;
    struct Snake *next;   
    int sayi;
} Snake;

typedef struct Food{
    int width;
    int height;
    char *color;
    int position_x;
    int position_y;
} Food ;

Snake *CreateNode(Snake *previous){
    Snake *node;
    node = (Snake *)malloc(sizeof(Snake));
    node->color = (*previous).color;
    node->height = (*previous).height;
    node->width = (*previous).width;
    node->sayi = (*previous).sayi;
    node->position_x = (*previous).position_x;
    node->position_y = (*previous).position_y;
    node->step = (*previous).step;   
    node->next = NULL;        
    return node;
}

void PrintList(Snake *root){
    Snake * tmp=root;
    while(tmp){
               if(tmp->sayi == 1){
                    DrawRectangle( 
                                (*tmp).position_x, 
                                (*tmp).position_y, 
                                (*tmp).width,
                                (*tmp).height,RED);  
                }else{

                    DrawRectangle( 
                                (*tmp).position_x, 
                                (*tmp).position_y, 
                                (*tmp).width,
                                (*tmp).height,GREEN);  

                }
      
        //Bir sonraki düğüm tmp değişkenine aktarılıyor.
        printf("sayi=> %d, xpos =>   %d- ypos =>%d\n", tmp->sayi, tmp->position_x,tmp->position_y);           
        tmp = tmp->next;
    }
  
    return;
}



// Random Feed Generator
 Food foods(Food *myfood,int position_x_range,  int position_y_range){

    time_t t; 
    srand((unsigned)time(&t));
    int rastgele = rand();

    (*myfood).position_x =  (rastgele % position_x_range);
    (*myfood).position_y =  (rastgele % position_y_range);
   
  return *myfood;
} 

void PrintFoods(Food *myfood){ 

    return DrawRectangle( 
                myfood->position_x, 
                myfood->position_y, 
                myfood->width,
                myfood->height,RED);   
}

 Snake *NodeYarat(Snake *onceki, Snake *root){    
    int yfark = 0;
    int xfark = 0;

    Snake *sonraki;
    sonraki = CreateNode(onceki);    
    onceki->next = sonraki;
    sonraki->sayi += 1;
    
    if(root->last_key_pressed == KEY_UP){        
        yfark = -root->step; 
        
    }

    if(root->last_key_pressed == KEY_DOWN){
        yfark = +root->step; 
        
    }

    if(root->last_key_pressed == KEY_LEFT){
        xfark = -root->step; 
        
    }

    if(root->last_key_pressed == KEY_RIGHT){ 
        xfark = +root->step; 
        
    }

    sonraki->position_x += xfark;   
    sonraki->position_y += yfark;
    return sonraki;
    
  }

 Snake *nodeRemove(Snake *root){
    // Bagli listeden bir eleman sil (en son eleman.)     
    (root->next)->last_key_pressed =(*root).last_key_pressed;
    free(root);
    root = root->next;    
    return root;
 }

 
 /*
    bool rangeOfMotion(Snake *root){

    Snake *tmp=root;
    while(tmp){

       
        printf("Yilanin Başı => %d, %d ", tmp->position_x,tmp->position_y); 
          

        tmp = tmp->next;
    }

    return true;
 }*/

 
 bool rangeOfMotion(Snake *temp,Snake *root,int Screen_width,int Screen_height){

        /*----------------------------------------------
        // Duvara Carpinca
        // 800, 0  x dogrultusu boyunca 
        // 0 , 600 y dogrultusu boyunca
        -----------------------------------------------*/
        
        if( !((
            temp->position_x > 0  &&   
            temp->position_x < Screen_width 
            ) 
            && 
            (
            temp->position_y > 0  &&   
            temp->position_y < Screen_height 
            )) 
        ){            
            return false;
        }

        Snake *tmp=root;
        while(tmp){
            if(abs(temp->position_x - tmp->position_x) <= temp->step  &&  abs(temp->position_y - tmp->position_y) <= temp->step   &&  temp->sayi - tmp->sayi > 15 ){   
                   
                   return false;                  
            }     
            tmp = tmp->next;   
        }



        return true;
 }

 int main(void)
 {   
    Snake * temp=NULL;
    Snake * node=NULL; 
    Snake * root=NULL;

    Snake mysnake;
        mysnake.height = 15;
        mysnake.width  = 15;
        mysnake.position_x = 200;
        mysnake.position_y = 200;
        mysnake.step = 5;
        mysnake.color = "#ff5733";
        mysnake.sayi= 1;

    Food myfood;
        myfood.height=15;
        myfood.width=15;
        myfood.position_x = 0;
        myfood.position_y = 0;
        myfood.color = "#aaabbb";

    int Screen_width = 800;
    int Screen_height = 600;
    InitWindow(Screen_width, Screen_height, "Snake Game V.0.1 - 2022.03.10");


    root = CreateNode(&mysnake);
   // printf("%d\n",root->sayi);


    SetTargetFPS(60);                // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop

   temp = NodeYarat(root,root);    

   myfood = foods(&myfood,Screen_width,Screen_height);  

   while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();

            ClearBackground(WHITE);             

            if(IsKeyPressed(KEY_UP) && (*root).last_key_pressed!= KEY_DOWN){
                (*root).last_key_pressed = KEY_UP;

            }

            if(IsKeyPressed(KEY_DOWN) && (*root).last_key_pressed!= KEY_UP){
                (*root).last_key_pressed = KEY_DOWN;                
            
            }

            if(IsKeyPressed(KEY_LEFT) && (*root).last_key_pressed!= KEY_RIGHT){
                (*root).last_key_pressed = KEY_LEFT;                
                    
            }

            if(IsKeyPressed(KEY_RIGHT) && (*root).last_key_pressed!= KEY_LEFT){
               (*root).last_key_pressed = KEY_RIGHT;
                
            }


            printf("\nSon Eleman %d,%d\n",temp->position_x,temp->position_y);  
            bool boom = rangeOfMotion(temp,root,Screen_width,Screen_height);
            if(boom == false){

              
            }else{

              temp = NodeYarat(temp,root);
              root = nodeRemove(root); 

            }

            // yilani 1 tık buyut , yemin konumunu degistir.
            if( abs(temp->position_x - myfood.position_x) <= temp->width  && abs(temp->position_y - myfood.position_y) <= temp->height ){  
                temp = NodeYarat(temp,root);
                myfood = foods(&myfood,Screen_width,Screen_height);  
                
            }
            

            PrintList(root);
            PrintFoods(&myfood); 
            DrawText("Snake By Kiyan Kaya \n Point ", 0, 0, 18, GREEN); 
         
            // Terminal Output => Snake Position
            // printf("x: %d ",(*root).position_x);   
            // printf("y: %d \n",(*root).position_y);  


        EndDrawing();  
        
    } 

    CloseWindow();

   free(root);

    return 0;
 }
