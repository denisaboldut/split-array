#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <setjmp.h>

typedef struct node                                         //definim structura pentru liste
{
    int val;
    struct node * next;
} node_t;


jmp_buf jump;//eticheta la care vom face salt atunci cand gasim solutia, o folosim in main
int index=0;

void push(node_t * head, int val)                          //functie pentru a adauga un element in lista
{
    node_t * current = head;
    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = (node_t *) malloc(sizeof(node_t));
    current->next->val = val;
    current->next->next = NULL;
}

void print_list(node_t * head)                              //functie pentru a printa listele
{
    node_t * current = head->next;

    while (current != NULL)
    {
        printf("%d ", current->val);
        current = current->next;
    }
}

bool procesareVector(int *vec, int size)                  // argumentul vec aici reprezinta o permutare a vectorului initial
{

    for(int k =1; k<size-1; k++)
    {
        int impartit = k;            //impartim vectorul pe rand, si anume
                                    //presupunem ca prima lista este formata dintr-un element, iar a doua din size-1 elemente
                                   //in urmatoarea iteratie, prima lista 2 elemente, a doua lista size-2 elemente ....

        int sum1=0;                                      //initializam variabile in care stocam suma
        int sum2=0;
        printf("\n");
            for(int i=0;i<size;i++){
                printf("%d ", vec[i]);
            }
        for(int i =0 ; i<impartit; i++)                  //calculam suma fiecarei parti din vector
        {
            sum1+= *(vec+i);
        }

        for(int j =impartit; j<size; j++)
        {
            sum2+=*(vec+j);

        }
        float m1=(float)sum1/(impartit);                 //calculam media aritmetica
        float m2=(float)sum2/(size-impartit);

        if (m1==m2)                                      //in cazul in care sunt egale, am gasit o solutie si returnam  true
        {                                               //pentru a ne opri din recursuvitate

            index=impartit;  //retinem indexul pana la care se formeaza prima lista
            return true;
        }
    }
    return false;

}

void swap(int *x, int *y)
{
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

void permute(int *vec,int inceput, int sfarsit )
{
    int i=0;

    node_t *listB = NULL;                            //declaram si initializam listele pentru output
    node_t *listC = NULL;
    listB = (node_t *) malloc(sizeof(node_t));
    listC = (node_t *) malloc(sizeof(node_t));

    if(inceput==sfarsit)                           //conditia de oprire a recursivitatii
    {

        bool stop=procesareVector(vec,sfarsit);  //apelam functia de procesare care va analiza cate o permutare a vectorului,
                                                //aceasta va returna valorea TRUE atunci cand putem imparti vectorul in doua parti
                                               //care respecta cerinta de a avea media artimetica egale

        if(stop==true)          //am gasit o varianta de impartire a vectorului, deci urmeaza sa ne oprim din recursivitate
        {
            printf("TRUE\n");

            for(int i=0; i<index; i++)           //parcurgem deci vectorul pana la indexul la care s-a gasit o solutie (prima lista)
            {
                push(listB,*(vec+i));             //si inseram in prima lista
            }

            for(int i=index; i<sfarsit; i++)      //restul elementelor care au ramas din permutarea respectiva
            {
                push(listC,*(vec+i));             //vor construi lista a doua
            }
            printf("\nList B:\n");
            print_list(listB);                    //afisam listele de output
            printf("\nList C:\n");
            print_list(listC);

            longjmp(jump,1);                    //pentru eficentizarea algoritmului in momentul in care gasim o solutie
                                               //ne oprim din recursivitate si facem jump in main

        }
        return;                               //altfel, functia de procesare nu a gasit nicio solutie inca deci
                                             //facem return din pasul de recursivitate
    }
    else
    {
        for(i=inceput; i<sfarsit; i++)
        {
            swap((vec+inceput),(vec+i));              //realizam permutarile in vector recursiv
            permute(vec,inceput+1,sfarsit);
            swap((vec+inceput),(vec+i));
        }
    }
}

int main()
{
    int size=1;                                            // initializam lungimea vectorului
    int *aux;                                              //vector auxiliar, folosit doar in cazul in care realocarea esueaza
    int *vec = malloc(sizeof(int)*size);                   //vectorul in care vom citi input-ul
    int i=0 ;

    printf("Insert array A here: \n");                     //numere intregi, la final urmat de orice simbol "/ . ! @ # .."
    while(scanf("%d",vec+i)==1)                            //cat timp avem ce citi de la tastura (numere)
    {
        i++;                                               //incrementam cursorul
        if(i==size)                                        //daca am ajuns deja la limita
        {
            aux = vec;
            size=size*2;
            vec=realloc(vec, size * sizeof(int));          //realocam memorie pentru a mai putea citi
            if(vec == NULL)                                //in caz de esec realocare
            {
                printf("Couldn't reallocate memory\n");
                size=aux;
                break;
            }
        }
        if(i==0)                                           //in caz de esec citire
        {
            printf("No input read\n");
            return 1;
        }
    }

    if(!setjmp(jump))                              //cat timp nu a avut loc saltul la eticheta se va face recursivitatea pentru permutari
        permute(vec,0,i);                         //apelam functia cu argumentele: vectorul, inceputul si lungimea lui
                                                 //facem permutari pe un vector pentru a putea gasi o varianta de impartire a acestuia
                                                //in care average(prima lista) == average(a doua lista)


    free(vec);                                        //eliberam din memorie spatiul alocat pentru vector
    vec=NULL;
    return 0;
}
