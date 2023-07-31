Grammar
=======

The grammar of stapl language is defined as follows:

.. code-block:: ebnf

   module = module_decl { item } ;
   module_decl = "module" identifier ;

   item = func_def | extern_func ;
   func_def = "def" , proto , compound_stmt ;
   extern_func = "extern" , proto ;

   proto = id , "(" , [ param_list ] , ")" , ":" , return_type ;
   type_name = "int" | "float" | "bool"
             | "array" , "[" , type_name , "," , int_literal "]"
             | "ptr" , "[" , type_name , "]" ;
   return_type = type_name | "void" ;
   param_list = param , { "," , param } ;
   param = id , ":" , type_name ;

   stmt = let_stmt | assign_stmt | if_stmt | return_stmt | compound_stmt ;
   let_stmt = "let" , id , ":" , type_name ;
   assign_operator = "=" ;
   assign_stmt = id , assign_operator , expr ;
   if_stmt = "if" , expr , compound_stmt , [ else , stmt ] ;
   return_stmt = "return" , expr ;
   compound_stmt = "{" , { stmt } , "}" ;

   expr = primary , binop_rhs | paren_expr ;
   primary = id | func_call | literal | paren_expr | array_subscript ;
   func_call = id , "(" , [ expr_list ] , ")" ;
   array_subscript = primary , "[" , int_literal , "]" ;
   expr_list = expr , { "," , expr } ;
   bool_literal = "true" | "false" ;
   literal = int_literal | float_literal | bool_literal ;
   binary_operator = "+" | "-" | "*" | "/" | "%"
                   | "==" | "!=" | ">" | ">=" | "<" | "<=" ;
   binop_rhs = { binary_operator , primary } ;
