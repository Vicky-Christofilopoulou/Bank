#include <iostream>
#include <string>
using namespace std;

class Cashier
{
    public:
        Cashier(int l);
        ~Cashier(){};
        void open();
        void close();
        void serve();
        bool free();
        bool is_open();
        bool is_free();
    private:
        bool open_flag;
        bool serving_flag;
        int customers_served;
        int L;
};

class Bank
{
public:
        Bank(int l, int k);           //Constructor
        ~Bank(){};                    //Destructor
        bool enter();
        bool serve();
        void exit();
        void open(int n);
        void close(int n);
        bool check_to_open();
        bool check_to_close();
        void waiting_customers();
        void get_open_cashiers();

    private:
        int last_customer;
        int curr_serving;
        int open_cashiers;
        int cashier_served;         //which cashier was the last one who served
        Cashier BankCashiers[5];
        int K;       
};

int main(int argc, char *argv[])
{
    int K;              //used in check_to_open, check_to_close
	int M;              
    int N;             //number of customers
    int L;              //L == customers_served -> cashier closed
    int i,j,k;           
    int not_served=0;
    int result;
    
    if (argc !=5)
    {
        cout<<"Wrong number of arguments. You must type 4 arguments."<<endl;
        exit(1);
    }
    K= atoi(argv[1]);
    M=atoi(argv[2]);
    N=atoi(argv[3]);
    L=atoi(argv[4]);

    Bank b(L,K);
    for (i=0; i<M; i++)
    {
        for (j=0; j<N; j++)
        {
            result=b.enter();
            if (result == false)
            {
                cout<<"Sorry you cannot enter until some customers are served!"<<endl;
                not_served++;
            }
            b.waiting_customers();
        }
        for (k=0; k<(N-not_served); k++) //only the ones who got it the bank 
        {
            result = b.serve();
            b.get_open_cashiers();
        }
    }

    cout << endl << "Repeating the process for the waiting customers." << endl << endl;
    for (i=0; i<not_served; i++)
    {
        b.enter();
        b.serve();
    }
    return 0;
}



//Funcions for Cashier
Cashier::Cashier(int l) : L(l)
{
    open_flag= false;
    serving_flag=false;
}

void Cashier:: open()
{
    open_flag=true;
    customers_served=0;
}

void Cashier::close()
{
    open_flag=false;
}

void Cashier::serve()
{
    serving_flag=true;
}

bool Cashier::free()
{
    serving_flag=false;
    customers_served++;
    if (customers_served == L)
    {
        close();
        cout<<"Cashier is closed because it served "<< L <<" people."<<endl;
    }
    return open_flag;
}

bool Cashier:: is_open()
{
    return open_flag;
}

bool Cashier::is_free()
{
    return serving_flag;
}


//Funcions for Bank
Bank::Bank(int l, int k): K(k), BankCashiers({l, l, l, l, l})
{
    last_customer=1;
    curr_serving=1;
    open(0);
    open_cashiers =1;
}

bool Bank::enter()
{
    int i;
    if (check_to_open() == false)        //checking if i need to open a cashier
    {
        last_customer++;
        return true;
    }   
    else
    {
        for (i=0; i<5; i++)
        {
            if (BankCashiers[i].is_open() == false)
            {
                open(i);
                open_cashiers++;
                last_customer++;
                return true;
            }
        }
        return false;
    }
}

bool Bank::serve()
{
    bool result;
    if (open_cashiers==0 )  //All cashiers are closed
    {
        open(0);
        open_cashiers++;
        cashier_served=0;
        cout<<"Customer No:"<<curr_serving<<" by cashier No:"<<cashier_served<<endl;
        BankCashiers[0].serve();
        result =BankCashiers[0].free();
        if (result == false)    //Cashier is closed
        {
            open_cashiers--;
        }
        exit();
        return true;
    }
    else
    {
        if (cashier_served==5) //the first cashier is going to serve
        {
            cashier_served=1;
        }
        else
        {
            cashier_served++;
        }
        for(int i=cashier_served; i<=5; i++)    //Checking if the next ones are open & free
        {
            if (BankCashiers[i-1].is_open() == true)   
            {
                cout<<"Customer No:"<<curr_serving<<" by cashier No:"<< i-1 <<endl;
                BankCashiers[i-1].serve();
                result = BankCashiers[i-1].free();
                if (result == false) //Cashier is closed
                {
                    open_cashiers--;
                }
                exit();
                return true;
            }
        }
        for (int i=0; i<cashier_served-1; i++) //Checking if the previous ones are open & free
        {
            if (BankCashiers[i].is_open() == true)   
            {
                cout<<"Customer No:"<<curr_serving<<" by cashier No:"<<i<<endl;
                BankCashiers[i].serve();
                result=BankCashiers[i].free();
                if (result == false) //Cashier is closed
                {
                    open_cashiers--;
                }
                exit();
                return true;
            } 
        }     
    }
    return false;
}

void Bank::exit()
{
    curr_serving++;
    if (check_to_close() ==true )
    {
        for (int i=0; i<5; i++)     //Close the first one which is open
        {
            if (BankCashiers[i].is_open() == true)
            {
                cout<<"Closing cashier No:"<< i << " because it is not necessary."<<endl;
                close(i);
                open_cashiers--;
                break;
            }
        }
    }
}

bool Bank::check_to_open()
{
    if ((last_customer - curr_serving) > open_cashiers * K)
    {
        return true;
    }
    else
        return false;
}

bool Bank::check_to_close()
{
    if ((last_customer - curr_serving) <= (open_cashiers-1) * K)
    {
        return true;
    }
    else 
        return false;
}

void Bank::open(int n)
{
    BankCashiers[n].open();
    cout<<"Cashier No:"<< n <<" is now open."<<endl;
}

void Bank::close(int n)
{
    BankCashiers[n].close();
    cout<<"Cashier No "<< n<< " closed."<<endl;
}

void Bank::waiting_customers()
{
    cout<<"Customers waiting to be served:"<< last_customer - curr_serving << endl;
}

void Bank::get_open_cashiers()
{
    cout<<"Open cashiers:"<< open_cashiers<<endl;
}