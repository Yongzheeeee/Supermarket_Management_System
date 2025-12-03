#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NUM 5

//       字符串   字符串   float   float   int
//商品   名字      编号    进价    售价    数量
struct item{   //商品
    char brand[20];   //商品名字
    char id[10];      //商品编号
    float in_price;   //进价
    float out_price;  //售价
    int storage;      //数量
};

//购物车类型
struct item_node{
    struct item wanted;
    int amount;
    struct item_node* next;
};

struct item goods[NUM];  //改数字可修改可存入的商品数量
struct item_node* cart;

//封装一个开始菜单
int menu(){

    int select;
    printf("请选择数字进行操作：\n");
    printf("1.建立库存信息\n");
    printf("2.显示库存信息\n");
    printf("3.购物车\n");
    printf("4.结算\n");
    printf("5.退出\n");
    printf("请选择对应的数字1--5:\n");

    //这样就可以实现无限次输入的操作
    while(1){
        scanf("%d",&select);
        if(select <1 || select>5){
            printf("输入错误，请重新输入!\n");
        }else{
            break; //直到满足条件
        }
    }
    return select;
}

//建立库存信息
void establish(){
    FILE* fp;   //创建了指向文件的指针
    printf("请输入商品信息：\n");
    printf("------------------------\n");
    
    //初始化商品信息存入goods数组中
    for(int i = 0; i<NUM; i++){
        printf("品名：");
        //fflush(stdin);   //清空缓冲区
        scanf("%*[^\n]%*c");
        scanf("%s",goods[i].brand);  //数组名就是首地址，不用加&

        printf("商品编号：");
        //fflush(stdin);   //清空缓冲区
        scanf("%*[^\n]%*c");
        scanf("%s",goods[i].id);

        printf("进价：");
        //fflush(stdin);   //清空缓冲区
        scanf("%*[^\n]%*c");
        scanf("%f",&goods[i].in_price);   //int 类型要加&

        printf("售价：");
        //fflush(stdin);   //清空缓冲区
        scanf("%*[^\n]%*c");
        scanf("%f",&goods[i].out_price);

        printf("数量：");
        //fflush(stdin);   //清空缓冲区
        scanf("%*[^\n]%*c");
        scanf("%d",&goods[i].storage);
        printf("\n");
    }
    
    //判断文件是否能够创建
    if((fp = fopen("goods", "w")) == NULL){
        printf("创建文件失败！\n");
        return;
    }

    //把goods数据的内容写入文件
    fwrite(goods, sizeof(struct item), NUM, fp);
    //要写入数据的起始地址   每个数据块的大小  要写入的数据块数量  文件指针

    //关闭文件
    fclose(fp);
}

//显示商品信息
void dis_all(){

    //清空前面显示的内容
    system("cls");

    FILE* fp; //指向文件的指针
    fp = fopen("goods","r");
    //            fread 目标地址  每个元素大小  要读取的元素数量   文件指针
    //          goods + i = goods的地址 + i * sizeof(struct item)
    for(int i=0;(fread(goods + i, sizeof(struct item),1,fp)!=0);i++){
        printf("--------------------------\n");
        printf("编号\t品名\t售价\t商品数量\n");
        printf("%s%9s%9.2f%9d\n",goods[i].id,goods[i].brand,goods[i].in_price,goods[i].storage);
    }
}

//购物车菜单
int cart_menu(){

    int select;
    printf("请选择操作：\n");
    printf("--------------------------\n");
    printf("1.显示当前购物列表\n");
    printf("2.添加商品\n");
    printf("3.退出\n");
    printf("--------------------------\n");

    //这样就可以实现无限次输入的操作
    while(1){
        scanf("%d",&select);
        if(select <1 || select>3){
            printf("输入错误，请重新输入!\n");
        }else{
            break; //直到满足条件
        }
    }
    return select;
}

//添加商品
void add(){
    FILE* fp;
    char str[20];
    int n;
    int i;
    char choice, choice2;
    struct item_node *p, *p1;
    do{
        printf("输入所需物品的名称或者编号：\n");
        //fflush(stdin);   //清空缓冲区
        scanf("%*[^\n]%*c");
        //gets(str);  //已废弃
        //fgets(str,sizeof(str), stdin);
        scanf("%s",str);
        if((fp=fopen("goods","r"))==NULL){
            printf("打开文件失败\n");
            continue;
        }
        for(i=0;fread(goods +i, sizeof(struct item),1,fp)!= 0; i++){
            //编号   名称  我们能对的上且库存数量不能等于0
            if(strcmp(goods[i].id, str) ==0 || strcmp(goods[i].brand, str) ==0 && goods[i].storage != 0){
               printf("已找到所需商品！\n");
               printf("---------------------------\n");
               printf("编号\t品名\t售价\t商品数量\n");
               printf("%s%9s%9.2f%9d\n",goods[i].id,goods[i].brand,goods[i].in_price,goods[i].storage);
               printf("请输入所需数量：\n");
               scanf("%d",&n);
               if(n>goods[i].storage){
                printf("库存不足！\n");
                break;
               }
               printf("是否购买？(Y/N)\n");
               //fflush(stdin);   //清空缓冲区
               scanf("%*[^\n]%*c");
               //scanf("%c",&choice);
               choice = getchar();  //只读单个字符
               if(choice =='Y' || choice == 'y'){
                p1 = (struct item_node*)malloc(sizeof(struct item_node));
                if(p1==NULL){
                    printf("内存申请失败!\n");
                    exit(1);
                }
                p1->amount = n;//存入需要购物的商品数量
                p1->wanted =goods[i];
                p1->next =NULL;
                p = cart;
                if(cart ==NULL){
                    cart = p1;
                }else{
                    while(p->next != NULL){
                        p = p->next;
                        // p1->next = p->next;
                        // p->next = p1;
                    }
                    p->next = p1;
                }
               }
               break;
            }
            
        }

        if(i ==NUM){
            printf("未找到所需物品！\n");
        }
        fclose(fp);
        printf("是否继续购物？(Y/N):\n");
        //scanf("%*[^\n]%*c");
        fflush(stdin);
        //scanf("%c",&choice2); 
        choice2 = getchar();
    }while(choice2 =='Y' || choice2 == 'y');   //无所谓choice2的值都能先运行一遍循环，这样可作为某种退出机制。
    
}

//显示购物车列表
void display(){
    struct item_node *p = cart;
    if(p == NULL){
        printf("购物车为空！\n");
        return;
    }
    while(p != NULL){
        printf("--------------------------\n");
        printf("编号\t品名\t售价\t数量\n");
        printf("%s%9s%9.2f%9d\n",p->wanted.id,p->wanted.brand,p->wanted.out_price,p->amount);
        p = p->next; //跳转到下一个节点
    }
}

//购物车
void shop_cart(){

    //清空前面显示的内容
    system("cls");
    
    while(1){
        switch (cart_menu())
        {
        case 1:
            //printf("1.显示当前购物列表\n");
            display();
            break;
        case 2:
            //printf("2.添加商品\n");
            add();
            break;
        case 3:  //printf("3.退出\n");
            return;
            break;
        }
    }

}

//结算功能实现
void calculate(){
    float total = 0, pay;
    struct item_node *p;
    FILE *fp;
    printf("以下是购物清单：\n");
    display();//显示购物车内容
    if((fp=fopen("goods","r")) == NULL){
        printf("文件打开失败！\n");
        return;
    }
    //从goods文件中读取最新的商品库存信息到goods数组中
    for(int j = 0;fread(goods + j, sizeof(struct item),1,fp)!= 0; j++){
        ;
    }
    fclose(fp);

    p = cart;
    while(p != NULL){
        total += p->wanted.out_price*p->amount;
        for(int i = 0;strcmp(goods[i].id,p->wanted.id) !=0;i++){
            goods[i].storage -= p->amount;
        }
        p = p->next;
    }
    printf("总计：%7.2f",total);
    printf("\n输入支付的金额:");
    scanf("%f",&pay);
    printf("实际支付:%7.2f\t\t找零:%7.2f\n",pay, pay-total);
    
    //将goods数组的最新数据写入到goods文件中去
    if(fopen("goods","w") == NULL){
        printf("文件打开失败！\n");
        return;
    }
    fwrite(goods,sizeof(struct item),NUM,fp);
    fclose(fp);

}

int main(){

    printf("*********************************************\n");
    printf("\t\t欢迎进入超市\n");
    printf("*********************************************\n");

    while(1){
        switch (menu())
        {
            case 1: //printf("1.建立库存信息\n");
                establish();
                break;
            case 2: //printf("2.显示库存信息\n");
                dis_all();
                break;
            case 3: //("3.购物车\n");
                shop_cart();
            break;
            case 4: //printf("4.结算\n");
                calculate();
                break;
            case 5:// printf("5.退出\n");
                printf("感谢您的使用，再见！\n");
                //exit(0);  //推出程序
                return 0;

        }
    }

    return 0;
}