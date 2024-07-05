#include <iostream>
#include <stack>
#include <string>
using namespace std;

int order(char operate){
    if(operate == '+'||operate == '-')
    {
        return 1;
    }
    if(operate == '*'||operate == '/')
    {
        return 2;
    }
    return 0;
}
 
int applyOperation(int a, int b, char operate){
    switch(operate){
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
    }
    return 0;
}

int evaluate(string expression){
    int i = 0;
    stack<int> numbers;
    stack<char> operators;

    while(i < expression.length()){
        if(expression[i] == '(')
        {
            operators.push(expression[i]);
        }
        else if(isdigit(expression[i]))
        {
            int val = 0;
            while(i < expression.length() && isdigit(expression[i]))
            {
                val = val * 10 + (expression[i] - '0');
                i++;
            }
            numbers.push(val);
            i--;
        }
        else if(expression[i] == ')')
        {
            while(!operators.empty() && operators.top() != '(' )
            {
                int val2 = numbers.top();
                numbers.pop();
                int val1 = numbers.top();
                numbers.pop();
                char operate = operators.top();
                operators.pop();
                numbers.push(applyOperation(val1, val2, operate));
            }

            if(!operators.empty())
            {
                operators.pop();
            }
        }
        else
        {
            while(!operators.empty() && order(operators.top()) >= order(expression[i])){
                int val2 = numbers.top();
                numbers.pop();
                int val1 = numbers.top();
                numbers.pop();
                char operate = operators.top();
                operators.pop();
                numbers.push(applyOperation(val1, val2, operate));
            }
            operators.push(expression[i]);
        }
        i++;
    }
     while(!operators.empty()){
        int val2 = numbers.top();
        numbers.pop();
        int val1 = numbers.top();
        numbers.pop();
        char operate = operators.top();
        operators.pop();
        numbers.push(applyOperation(val1, val2, operate));
    }
    return numbers.top();
}

int main(){
    cout << "Enter the expression to evaluate: ";
    string expression;
    getline(cin, expression);
    cout << evaluate(expression);
}