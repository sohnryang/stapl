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
   type_name = "int" | "float" | "bool" ;
   return_type = type_name | "void" ;
   param_list = param , { "," , param } ;
   param = id , ":" , type_name ;

   stmt = let_stmt | assign_stmt | if_stmt | while_stmt | jump_stmt
        | return_stmt | compound_stmt ;
   let_stmt = "let" , id , ":" , type_name ;
   assign_operator = "=" ;
   assign_stmt = id , assign_operator , expr ;
   if_stmt = "if" , expr , compound_stmt , [ else , stmt ] ;
   while_stmt = "while" , expr , compound_stmt ;
   jump_stmt = "break" | "continue" ;
   return_stmt = "return" , expr ;
   compound_stmt = "{" , { stmt } , "}" ;

   primary = id | func_call | literal | paren_expr ;
   unary_operator = "+" | "-" | "!" ;
   unary_expr = unary_operator , unary_expr | primary ;
   expr = unary_expr , binop_rhs | paren_expr ;
   func_call = id , "(" , [ expr_list ] , ")" ;
   expr_list = expr , { "," , expr } ;
   bool_literal = "true" | "false" ;
   literal = int_literal | float_literal | bool_literal ;
   binary_operator = "+" | "-" | "*" | "/" | "%"
                   | "==" | "!=" | ">" | ">=" | "<" | "<=" ;
   binop_rhs = { binary_operator , unary_expr } ;
