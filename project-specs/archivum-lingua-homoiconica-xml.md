<!-- ==================================================
  ARCHIVUM — fons primarius, verbatim servatus.

  Fran's design notes for the full homoiconic XML-based
  C-targeting language — sibling/successor of the XML Macro
  System notes (see archivum-xml-macro-system.md), predecessor of
  STML. Previously unwritten in any repo; recovered and pasted
  into the 2026-08-10 conversation. Per-idea status verdicts live
  in project-specs/stml-visio.md — notably: the arity markers,
  capture operators, and fragments SHIPPED into STML; `<&>` as
  address-of prefigured the `&nomen;` individuum sigil; the
  expression plane (infix XML, positional macro args) is MORTUUM
  (C89 + silva won). Read stml-visio.md before building from here.
  ================================================== -->

# Complete XML-Based Language Definition

## Core Syntax Elements

### Variable References and Declarations

```xml
<!-- Variable reference -->
<@varname />
<@varname pointer/>                    <!-- Dereference pointer -->
<@varname "member" />                  <!-- Member access (struct.member) -->
<@varname "member1" "member2" />       <!-- Chained member access -->

<!-- Variable declaration with optional initialization -->
<@varname type="int" />                <!-- Declaration only -->
<@varname type="int">42</@varname>    <!-- Declaration with initialization -->
<@varname type="int" global="true" /> <!-- Global variable -->

<!-- Array declaration -->
<@buffer type="char" array-size="100" />
```

### Parameter Declaration vs Invocation

```xml
<!-- In function definition: single colon for parameters -->
<function name="example">
  <:param1 type="int" />
  <:param2 type="char*" />
  <:... />                             <!-- Variadic parameters -->
</function>

<!-- In macro invocation: double colon for named arguments -->
<when>
  <::test><@x /> </ greater-than /> 5</::test>
  <::body>
    <print>x is greater than 5</print>
  </::body>
</when>
```

### Child Gathering Notations

```xml
<!-- Traditional nesting -->
<add>
  <@x />
  <@y />
</add>

<!-- Arity markers -->
<add (> <@x />                         <!-- Unary: takes next element -->
<add ((> <@x /> <@y />                 <!-- Binary: takes next 2 elements -->
<add (((> <@x /> <@y /> <@z />         <!-- Ternary: takes next 3 elements -->

<!-- Infix notation -->
<@x /> </ add /> <@y />                <!-- Binary infix operator -->

<!-- Postfix notation -->
<@x /> <) post-inc>                    <!-- Postfix operator -->
```

### Array Access and Grouping

```xml
<!-- Array access -->
<@arr[> <@index /> <]>                 <!-- Single dimension -->
<@matrix[> <@i /> <][> <@j /> <]>     <!-- Multi-dimensional -->

<!-- Expression grouping (parentheses) -->
<{> <@x /> </ add /> <@y /> <}/>       <!-- (x + y) -->

<!-- Complex example -->
<@arr[> <{> <@i /> </ multiply /> 2 <}/> <]>  <!-- arr[i * 2] -->
```

### Literals

```xml
<!-- Numeric literals (bare text) -->
42
3.14
0xFF

<!-- String literals (bare text) -->
Hello, World!
Enter your name:

<!-- Character literals -->
'A'
'\n'
'\0'

<!-- Explicit type casting -->
<int (> 42
<string (> Hello
<cast type="void*"> <@ptr /> </cast>
```

### Control Flow

```xml
<!-- If statement -->
<if>
  <condition (> <@x /> </ greater-than /> 0
  <@y /> </ assign /> 10
</if>

<!-- If-else with chaining -->
<if>
  <condition (> <@x /> </ equal /> 0
  <print>Zero</print>

<else-if (>
  <condition (> <@x /> </ greater-than /> 0
  <print>Positive</print>

<else>
  <print>Negative</print>
</else>
</else-if>
</if>

<!-- While loop -->
<while>
  <condition (> <@i /> </ less-than /> 10
  <@i /> <) post-inc>
</while>

<!-- For loop -->
<for>
  <init> <@i type="int" (> 0 </init>
  <condition> <@i /> </ less-than /> 10 </condition>
  <update> <@i /> <) post-inc> </update>
  <print><@i /></print>
</for>
```

### Function Definition and Calls

```xml
<!-- Function definition -->
<function name="add" returns="int">
  <:a type="int" />
  <:b type="int" />
  <return (> <@a /> </ add /> <@b />
</function>

<!-- Function calls -->
<add ((> 5 10                          <!-- Binary function -->
<printf (> Hello, World!               <!-- Unary function -->
<fprintf (((> <@stderr /> Error: %s\n <@msg />  <!-- Ternary -->

<!-- Forward declaration -->
<function-decl name="helper" returns="void" />
```

### Operators

```xml
<!-- Arithmetic (infix) -->
<@a /> </ add /> <@b />
<@a /> </ subtract /> <@b />
<@a /> </ multiply /> <@b />
<@a /> </ divide /> <@b />
<@a /> </ modulo /> <@b />

<!-- Comparison (infix) -->
<@a /> </ equal /> <@b />
<@a /> </ not-equal /> <@b />
<@a /> </ less-than /> <@b />
<@a /> </ greater-than /> <@b />
<@a /> </ less-equal /> <@b />
<@a /> </ greater-equal /> <@b />

<!-- Logical (infix) -->
<@a /> </ logical-and /> <@b />
<@a /> </ logical-or /> <@b />
<not (> <@condition />

<!-- Assignment -->
<@x /> </ assign /> 42
<@x /> </ add-assign /> 10
<@x /> </ multiply-assign /> 2

<!-- Postfix -->
<@i /> <) post-inc>
<@i /> <) post-dec>

<!-- Address-of -->
<&> <@variable /> </&>
```

## Macro System

### Basic Macro Definition

```xml
<defmacro>
  <:macro-name>
    <:param1/>
    <:param2/>
    <:rest.../>                        <!-- Rest parameters -->
  </:macro-name>
  <!-- Macro body returns AST -->
</defmacro>
```

### Variable Interpolation in Macros

```xml
<@variable/>                           <!-- Interpolate value -->
<@!variable/>                          <!-- Quote (prevent evaluation) -->
<@...variable/>                        <!-- Splice array/list -->

<!-- Positional arguments -->
<@$1/>                                 <!-- First positional -->
<@$2/>                                 <!-- Second positional -->
<@$/>                                  <!-- All positional as list -->
<@...$/>                               <!-- Splice all positional -->
```

### AST Mutation Operations

```xml
<!-- Removal -->
<%remove/>                             <!-- Remove matched element -->
<%remove-parent/>                      <!-- Remove parent -->
<%empty/>                              <!-- Remove all children -->

<!-- Replacement -->
<%replace-with>new content</%replace-with>
<%replace-content>new children</%replace-content>

<!-- Insertion -->
<%before>content</%before>
<%after>content</%after>
<%prepend>first child</%prepend>
<%append>last child</%append>

<!-- Wrapping -->
<%wrap>wrapper element</%wrap>
<%unwrap/>                             <!-- Remove parent, keep element -->

<!-- Attributes -->
<%set-attr name="value"/>
<%remove-attr name="attr"/>
```

### Pattern Matching with Selectors

```xml
<!-- CSS-style selectors -->
<--> function[name='main'] </-->       <!-- Attribute match -->
<--> function > return </-->           <!-- Child selector -->
<--> :not(:has(doc-comment)) </-->     <!-- Pseudo-selectors -->

<!-- AND combination -->
<-->
  function,
  [visibility='public'],
  :has(return)
</-->

<!-- Pattern extraction with $ variables -->
<select>
  <--> assign[target='$var'] > literal[value='$val'] </-->
  <!-- $var and $val are now available -->
</select>
```

### Transformations

```xml
<transform>
  <@ast/>

  <select>
    <--> selector </-->
    <%operation/>
  </select>
</transform>

<!-- Pipeline transformations -->
<pipeline>
  <@ast/>
  <find-all><--> function </--></find-all>
  <filter><is-public/></filter>
  <map><%add-logging/></map>
</pipeline>
```

### Transclusion

```xml
<!-- Include fragment by ID -->
<< #fragment-id >>

<!-- Include with CSS selector -->
<< function[name='helper'] >>

<!-- Include with transformation -->
<< #fragment | transform >>
```

## Fragments and Documentation

```xml
<!-- Named fragment (not in output until transcluded) -->
<#my-fragment>
  <@x /> </ multiply /> 2
</#my-fragment>

<!-- Anonymous documentation fragment -->
<#>
  <title>My Program</title>
  <author>Developer</author>
  <description>
    This fragment can contain any semantic markup
    and is ignored during execution.
  </description>
</#>

<!-- Parameterized fragment -->
<#binary-op operator="@op">
  <@left /> </ @op /> <@right />
</#binary-op>

<!-- Using parameterized fragment -->
<< #binary-op operator="add" >>
```

## Complete Example

```xml
<program>
  <#>
    <title>Factorial Calculator</title>
    <description>Recursive and iterative implementations</description>
  </#>

  <include (> stdio.h

  <!-- Define a macro for assertions -->
  <defmacro>
    <:assert>
      <:condition/>
      <:message/>
    </:assert>
    <if>
      <not><@condition/></not>
      <do>
        <fprintf ((> <@stderr /> Assertion failed: <@message />
        <exit (> 1
      </do>
    </if>
  </defmacro>

  <!-- Recursive implementation -->
  <function name="factorial_recursive" returns="int">
    <:n type="int" />

    <assert>
      <::condition><@n /> </ greater-equal /> 0</::condition>
      <::message>n must be non-negative</::message>
    </assert>

    <if>
      <condition (> <@n /> </ less-equal /> 1
      <return (> 1
    </if>

    <return (> <@n /> </ multiply /> <factorial_recursive (> <@n /> </ subtract /> 1
  </function>

  <!-- Fragment for iterative implementation -->
  <#iterative-loop>
    <while>
      <condition (> <@i /> </ less-equal /> <@n />
      <@result /> </ multiply-assign /> <@i />
      <@i /> <) post-inc>
    </while>
  </#iterative-loop>

  <!-- Iterative implementation -->
  <function name="factorial_iterative" returns="int">
    <:n type="int" />

    <@result type="int" (> 1
    <@i type="int" (> 1

    << #iterative-loop >>

    <return (> <@result />
  </function>

  <!-- Apply optimization transformation -->
  <transform>
    <@factorial_recursive/>
    <select>
      <--> function > if > condition > literal[value='1'] </-->
      <%replace-with>
        <@n /> </ equal /> 0 </ logical-or /> <@n /> </ equal /> 1
      </%replace-with>
    </select>
  </transform>

  <function name="main" returns="int">
    <printf ((> 5! = %d\n <factorial_recursive (> 5
    <return (> 0
  </function>
</program>
```

This language combines:
1. Clean syntax for common C patterns
2. Powerful macro system with AST manipulation
3. CSS-style selectors for pattern matching
4. Literate programming with fragments
5. Flexible operator notations
6. Built-in transformation capabilities

The result is a homoiconic language that can express both low-level C code and high-level metaprogramming in a unified syntax.
