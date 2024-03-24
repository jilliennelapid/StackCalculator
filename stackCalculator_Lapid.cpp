#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>

#define MAX 100
using namespace std;

// Template class to allow different data types to be stored in the stack.
template<typename T>
class Stack
{
  public:
    int top;                  // Indicates the index of the top element in the stack.
    T stackArray[MAX];        // Stack data is stored in a template type array.
    string postfix;           // Holds the postfix expression.
    bool flag = false;        // Flag to indicate passing through parts of logic.

    // Arrays used for checking for operators, operands, and parentheses.
    char operators[6] = {'^', '*', '/', '%', '+', '-'};
    char operands[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    char parentheses[6] = {'(', '{', '[', ')', '}', ']'};

    // Constructor
    Stack() 
    {
      top = -1;
    }

    // Class Methods:
    // push() : pushes data into the stack.
    void push(const T& data) 
    {
      if(top >= (MAX - 1))
      {
        return;
      }
      else
      {
        ++top;

        stackArray[top] = data;
      }
    }

    // pop(): pops data out of the stack, returns the value popped.
    T pop()
    {
      if(top < 0)
      {
        return -15;
      }
      else
      {
        T x = stackArray[top];

        --top;

        return x;
      }
    }

    // checkForOperator(): checks if the passed in character is an operator, 
    /// returning either true or false.
    bool checkForOperator(char optr)
    {
      for(int i = 0 ; i < sizeof(operators); i++)
      {
        if(operators[i] == optr)
        {
          return true;
        }
      }

      return false;
    }

    // checkForOperand(): checks if the passed in character is an operand, 
    /// returning either true or false.
    bool checkForOperand(char oprd)
    {
      for(int i = 0; i < sizeof(operands); i++)
      {
        if(operands[i] == oprd)
        {
          return true;
        } 
      }

      return false;
    }

    // checkForParentheses(): checks if the passed in character is a form of parentheses, 
    /// returning either true or false.
    bool checkForParentheses(char oprd)
    {
      for(int i = 0; i < sizeof(parentheses); i++)
      {
        if(parentheses[i] == oprd)
        {
          return true;
        } 
      }
    
      return false;
    }
};

// areParenthesesBalanced(): checks for proper closure of parentheses, braces, or brackets.
bool areParenthesesBalanced(string expression)
{
  int ptCount = 0;
  int brCount = 0;
  int bkCount = 0;

  for(int i = 0; i < expression.size(); i++)
  {
    char pt = expression.at(i);

    if(pt == '(' || pt == ')')
    {
      ptCount++;
    }
    else if (pt == '{' || pt == '}')
    {
      brCount++;
    }
    else if(pt == '(' || pt == ')')
    {
      bkCount++;
    }
  }

  if((ptCount % 2) == 0 && (brCount % 2) == 0 && (bkCount % 2) == 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

// givePriority(): used to dictate the priority of operands,
/// [3] = ^ || [2] = *, /, % || [1] = +, -
int givePriority(int eval)
{
  int E = 0;
  
  if(eval == 1 || eval == 2 || eval == 3)
  {
    E = 2;
  }
  if(eval == 4 || eval == 5)
  {
    E = 1;
  }
  else
  {
    E = 3;
  }
  
  return(E);
}

// pushToStack(): handles pushing and popping operators and parentheses.
void pushToStack(Stack<char>* stack, char optr)
{ 
  // Immediately pushes an operand if it is the first operator,
  /// or if it is an opening parentheses.
  if(stack->top == -1 || optr == '(' || optr == '{' || optr == '[')
  {
    stack->push(optr);

    if(optr == '(' || optr == '{' || optr == '[')
    {
      stack->flag = true;
    }
    return;
  }

  // Pops all elements if a closing parentheses is encountered.
  if(optr == ')' || optr == '}' || optr == ']')
  {
    stack->flag = false;

    for(int e = stack->top; e > -1; e--)
    {
      if(stack->stackArray[e] == '(' || stack->stackArray[e] == '{' || stack->stackArray[e] == '[')
      {
        stack->pop();
        return;
      }
      else
      {
        stack->postfix += stack->pop();
      }
    }
  }

  // The following code handles when an operator is already in the stack 
  /// and a new one has been encountered in the infix expression:

  // 1) Determine the priority of the operators:
  int in_stack;
  int to_add;

  /// Finds there equivalent position in the array operators[],
  //// stores those values.
  for(int j = 0; j < sizeof(stack->operators); j++)
  {
    int n = stack->top;
    
    if(stack->stackArray[n] == stack->operators[j])
    {
      in_stack = j;
    }
    else if(optr == stack->operators[j])
    {
      to_add = j;
    }
  }

  /// Uses givePriority() to get the priority of based on their indices.
  int stack_val = givePriority(in_stack);
  int current_val = givePriority(to_add);

  
  // 2) Determine how to handle to operators based on their priority.
  /// If another set of parentheses is found, immediately push.
  if(stack->flag == true && stack->stackArray[stack->top] == '(' || stack->stackArray[stack->top] == '{' || stack->stackArray[stack->top] == '[')
  {
    stack->push(optr);
    return;
  }
  /// If the current operator is lesser or same priority as the operator
  //// in the stack, then pop the stack operator and push the current operator.
  if(current_val <= stack_val)
  {
    stack->postfix += stack->pop();
    stack->push(optr);
  }
  /// Otherwise, the operator is of greater priority than the current
  //// operator, so it is also added into the stack.
  else
  {
    stack->push(optr);
  }  
}

// evalPostfix(): evaluates the postfix expression using a temporary
/// double stack to store the operands and operation results.
void evalPostfix(Stack<char>* stack)
{
  // Initializing a temporary stack.
  Stack<double>* temp = new Stack<double>();

  // for loop to iterate throught the entire postfix.
  for(int i = 0; i < stack->postfix.size(); i++)
  {
    // Stores the current character.
    char lett = stack->postfix.at(i);

    // If the current character is an operand,
    /// convert char -> string -> double and push into temp.
    if(temp->checkForOperand(lett) == true)
    {
      string num(1, lett);
      double convert = stod(num);
      temp->push(convert);
    }
    // Else, if the current character is an operator,
    /// the operands can be popped out of temp and a switch
    /// statement helps determine the correct operation to perform.
    else if(temp->checkForOperator(lett) == true)
    {
      double B = temp->pop();
      double A = temp->pop();
      
      double result;
      char optr = stack->postfix.at(i);

      switch (optr)
      {
        case '^':
          result = pow(A, B);
          temp->push(result);
          break;

        case '*':
          result = A * B;
          temp->push(result);
          break;

        case '/':
          result = A / B;
          temp->push(result);
          break;

        case '%':
          result = fmod(A, B);
          temp->push(result);
          break;

        case '+':
          result = A + B ;
          temp->push(result);
          break;

        case '-':
          result = A - B;
          temp->push(result);
          break;

        default:
          cout << "Error" << endl;
          break;  
      }
    }
    // Otherwise, there is an invalid number of operands.
    else
    {
      cout << "Invalid number of operands." << endl;
      return;
    }
  }

  // Pops out the final calculated value. Formats the output
  /// to a precision of 3 and then returns the output to the user.
  double output = temp->pop();
  cout << fixed << showpoint;
  cout << "The expression equals: " << setprecision(3) << output << endl;

  // De-allocates temp for space.
  delete temp;
}

// expressionToPostfix(): Takes in the infix expression and converts it to postfix.
void expressionToPostfix(Stack<char>* stack, string expression)
{
  // Uses areParenthesesBalanced() to ensure closure of all parentheses.
  if(areParenthesesBalanced(expression) == false)
  {
    cout << "Invalid Parentheses." << endl;
    return;
  }

  // Counter to determine invalid operands or not.
  int oprdCounter = 0;

  // Iterates through the infix expression.
  for(int i = 0; i < expression.size(); i++)
  {
    // Stores the current character.
    char lett = expression.at(i);

    // If the character is an operand, immediately add the character to postfix.
    if(stack->checkForOperand(lett) == true)
    {
      oprdCounter++;
      
      stack->postfix += lett;
      continue;
    }
    // Else, if the postfix has no operands and an operator is trying to be added,
    /// this will generate an error.
    else if(stack->postfix.size() == 0 && stack->checkForOperator(lett) == true)
    {
      cout << "Invalid number of operands." << endl;
      return;
    }
    // Else, if there are two types of operators near one another, this is another error.
    else if(stack->checkForOperator(lett) == true && stack->checkForOperator(expression.at(i-1)) == true)
    {
      cout << "Invalid number of operators." << endl;
      return;
    }
    // Else, if the character is an operator or a parentheses, the character is sent to
    /// pushToStack() to be properly added into the stack.
    else if(stack->checkForOperator(lett) == true || stack->checkForParentheses(lett) == true)
    {
      char n = lett;

      pushToStack(stack, n);
    }
    // Else, if there is a space " " character in the expression, this is an error.
    else if(lett == ' ')
    {
      cout << "Invalid number of operators." << endl;
      return;
    }
    // If not any of the above, then the character is a letter or special character
    /// which is not permitted in the expression.
    else
    {
      cout << "Invalid character '" << lett << "' in the expression." << endl;
      return;
    }
  }

  // Pops the rest of the stack contents when the whole infix is evaluated.
  while(stack->top != -1)
  {
    stack->postfix += stack->pop();
  }

  // If there are 0-1 operands in the expression, then the expression does
  /// not have enough operands and is an error.
  if(oprdCounter < 2)
  {
    cout << "Invalid number of operands." << endl;
    return;
  }

  // Shows the postfix conversion result to the user.
  cout << "Postfix expression: " << stack->postfix << endl;

  // Sends the postfix to be evaluated for a final result.
  evalPostfix(stack);
}


// main(): Uses a while loop to allow the user to continuously input
/// expressions until they enter "++" to quit the program.
int main() 
{
  cout <<"Calculator App" << endl;

  // Stores the expression from the user, or the infix.
  string infix;

  // Allows the user to keep evaluating expressions until the user exits.
  while(infix != "++")
  {
    // Clears Infix.
    infix = "";
    // Intializes a stack.
    Stack<char> stk;

    // Prints out the prompts the user, takes in the user-input via getline().
    cout << "__________________________________________" << endl;
    cout << "Type ++ to Quit.\n"  << "Enter an expression to evaluate: ";
    getline(cin, infix);

    // Stops the loop when "++" is inputted.
    if(infix == "++")
    {
      cout << "-Quitting Calculator-";
      continue;
    }
    // Otherwise, the expression (infix) can be sent to be tested.
    else
    {
      expressionToPostfix(&stk, infix);
    }
  }
}