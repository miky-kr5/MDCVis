MDCVis coding style guidelines
==============================

This document details the coding style to be used throughout the entire MDCVis project. It is based on the [id Software] [1] Coding Style Conventions available on the public [id Software FTP server] [2].

GENERAL
-------
Use real tabs that equal 4 spaces.  

Use typically trailing braces everywhere (if, else, functions, structures, typedefs, class definitions, etc.)

    if ( x ) {
    }

The else statement starts on the same line as the last closing brace.

    if ( x ) {
    } else {
    }

Pad parenthesized expressions with spaces

    if ( x ) {
    }

Instead of

    if (x) {
    }

And

    x = ( y * 0.5f );

Instead of

    x = (y * 0.5f);

Use precision specification for floating point values unless there is an explicit need for a double.

    float f = 0.5f;

Instead of

    float f = 0.5;

And

    float f = 1.0f;

Instead of

    float f = 1.f;

Function names start with a lower case:

    void function( void );

In multi-word function names each word starts with an upper case except the first word:

    void thisFunctionDoesSomething( void );

Variable names start with a lower case character.

    float x;

In multi-word variable names the first word starts with a lower case character and each successive word starts with an upper case.

    float longVariableName;

Typedef names use the same naming convention as variables, however they always end with "_t".

    typedef int newTypeName_t;

Struct names use the same naming convention as variables, however they always end with "_t".

    struct structDefinition_t;

Enum names use the same naming convention as variables, however they always end with "_t". The enum constants use all upper case characters. Multiple words are separated with an underscore.

    enum enumeratedType_t {
        VALUE_A,
        VALUE_B,
        VALUE_C,
        VALUE_D
    };

Names of recursive functions end with "_r"

    void recursiveFunction_r( int parameter );

Defined names use all upper case characters. Multiple words are separated with an underscore.

    #define DEFINED_NAME 0

Use ‘const’ as much as possible.  

Use:

    const int *p;           // pointer to const int
    int * const p;          // const pointer to int
    const int * const p;    // const pointer to const int

Don’t use:

    int const *p;

COMMENTS
--------

Every file must include the follwing header text at the beggining:

    /*------------------------------------------------------------------------------
    ; File:          <file name with file extension>
    ; Description:   <short one line description of the contents of the file.
    ; Author:        <name of author>, <email>@<some server>
    ; Date created:  <date the file was created in mm/dd/yyyy format>
    ;
    ; Copyright (C) <Year of file creation> Fundacion Museos Nacionales
    ;
    ; This program is free software: you can redistribute it and/or modify
    ; it under the terms of the GNU General Public License as published by
    ; the Free Software Foundation, either version 3 of the License, or
    ; (at your option) any later version.
    ;
    ; This program is distributed in the hope that it will be useful,
    ; but WITHOUT ANY WARRANTY; without even the implied warranty of
    ; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    ; GNU General Public License for more details.
    ;
    ; You should have received a copy of the GNU General Public License
    ; along with this program.  If not, see <http://www.gnu.org/licenses/>.
    ;-----------------------------------------------------------------------------*/


The standard comment header for a class is:

    /*------------------------------------------------------------------------------
    ; Description
    ;-----------------------------------------------------------------------------*/

The standard comment header for functions is:

    /*------------------------------------------------------------------------------
    ; FunctionName
    ; Description
    ;-----------------------------------------------------------------------------*/

Do not use C style comments inside of functions. Breakdown multiline comments using many single line C++ style comments one after the other.
Use:

    void someFunction( void ){
       // Short one line comment.

       // Longer comments are
       // broken down into
       // separate lines
    }

Instead of:

    void someFunction( void ){
       /* Short one line comment. */

       /* Longer comments are
          broken down into
          separate lines */
    }

CLASSES
-------
Class names start with "mdc" and each successive word starts with an upper case.

    class mdcVec3;

Class variables have the same naming convention as variables.

    class mdcVec3{
        float x;
        float y;
        float z;
    }

Class methods have the same naming convention as functions.

    class mdcVec3{
        float length( void ) const;
    }

Indent the names of class variables and class methods to make nice columns. The variable type or method return type is in the first column and the variable name or method name is in the second column.

    class mdcVec3{
        float          x;
        float          y;
        float          z;
        float          length( void );
        const float *  toFloatPtr( void ) const;
    }

The * of the pointer is in the first column because it improves readability when considered part of the type.  

Ording of class variables and methods should be as follows:

1. list of friend classes
2. public variables
3. public methods
4. protected variables
5. protected methods
6. private variables
7. private methods

This allows the public interface to be easily found at the beginning
of the class.  

Always make class methods ‘const’ when they do not modify any class variables.

Avoid use of ‘const_cast’. When an object is needed to be modified, but only const versions are accessible, create a function that clearly gives an editable version of the object. This keeps the control of the ‘const-ness’ in the hands of the object and not the user. Return ‘const’ objects unless the general usage of the object is to change its state. For example, media objects should be const to a majority of the code, while entity objects tend to have their state modified by a variety of systems, and so are ok to leave non-const.  

Function overloading should be avoided in most cases. For example, instead of:

    const mdcObject * getObject( int index ) const;
    const mdcObject * getObject( const char *name ) const;

Use:

    const mdcObject * getObjectByIndex( int index ) const;
    const mdcObject * getObjectByName( const char *name ) const;

Explicitly named functions tend to be less prone to programmer error and inadvertent calls to functions due to wrong data types being passed in as arguments. Example:

    object = getObject( 0 );

This could be meant as a call to get a random animation, but the compiler would interpret it as a call to get one by index.

Overloading functions for the sake of adding ‘const’ accessible function is allowable:

    class mdcMutableObjectManager : public mdcManager {
        mdcMutableObject * getMutableObject( int index );
        const mdcMutableObject * getMutableObject( int index ) const;
    };

In this case, a const version of getMutableObject was provided in order to allow getMutableObject to be called from const functions. This is allowable in known mutable classes which are normally non const. For inmutable objects, which are normally const, operator overloading should be avoided.

STANDARD CLASS NAMES
--------------------

    mdc<name>Dlg      // dialog class
    mdc<name>Ctrl     // dialog control class
    mdc<name>Frm      // frame window
    mdc<name>View     // view window
    mdc<name>Mdl      // model (as in the Model-View-Controller pattern) class
    mdc<name>         // any other class

FILE NAMES
---------
Each class should be in a seperate source file unless it makes sense to group several smaller classes.  
The file name should be the same as the name of the class without the "mdc" prefix. (Upper/lower case is preserved.)

    class mdcClass;

    files:

    Class.cpp
    Class.h

When a class spans across multiple files these files have a name that starts with the name of the class without "mdc", followed by an underscore and a subsection name.

    class mdcHugeClass;
    
    files:
    
    HugeClass_moduleA.cpp
    HugeClass_moduleB.cpp
    HugeClass_moduleC.cpp
    HugeClass.h

When a class is a public virtual interface to a subsystem the public interface is implemented in a header file with the name of the class without "mdc". The definition of the class that implements the subsystem is placed in a header file with the name of the class without "mdc" and ends with "_local.h". The implementation of the subsystem is placed in a cpp file with the name of the class without "mdc".

    class mdcVirtualClass;
    
    VirtualClass.h            // public virtual mdcVirtualClass interface
    VirtualClass_local.h      // definition of class mdcVirtualInterfaceClassLocal
    VirtualClass.cpp          // implementation of mdcVirtualClassLocal

  [1]: http://www.idsoftware.com/ "id Software"
  [2]: ftp://ftp.idsoftware.com/idstuff/doom3/source/CodeStyleConventions.doc "id Software FTP server"
