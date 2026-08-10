<!-- ==================================================
  ARCHIVUM — fons primarius, verbatim servatus.

  Fran's design notes for the "XML Macro System" — a predecessor
  of STML from the "XML extension" era. Previously unwritten in
  any repo; recovered from Fran's notes and pasted into the
  2026-08-10 conversation that produced the necessitudines arc,
  the signa migration (# -> &nomen;), and project-specs/
  stml-visio.md. The vision doc carries the per-idea STATUS
  verdicts; THIS file preserves the primary source so the full
  examples (defmacro, pipelines, the aspect-oriented stylesheet)
  survive for the future STML transform tool (stml-visio §6).

  Status of the system as a whole: ALIBI VIVIT by decomposition —
  selectors -> silva, %-verbs -> renominare/emendare, transclusion
  + capture -> STML, <#> documentation -> nota doctrine. The macro
  plane (defmacro/compile-eval) is MORTUUM in the data plane by
  house law. See stml-visio.md before building anything from here.
  ================================================== -->

# XML Macro System - Complete Syntax Documentation

## Table of Contents
1. Overview
2. Core Syntax Elements
3. Macro Definition
4. AST Manipulation
5. Pattern Matching & Selectors
6. Transformations
7. Compilation & Evaluation
8. Module System
9. Complete Examples

## Overview
The XML Macro System is a powerful metaprogramming system that treats XML as both code and data, enabling Lisp-style macros with more intuitive syntax for AST manipulation.

### Key Innovations
* CSS-style selectors for AST queries
* Clear visual distinction between definition and invocation
* Pipeline-based transformations
* Bidirectional module system
* Natural handling of named and positional arguments

## Core Syntax Elements

### Variable Interpolation
```xml
<!-- Basic interpolation -->
<@variable/>                <!-- Interpolate variable value -->
<@!variable/>               <!-- Quote variable (prevent evaluation) -->
<@...variable/>             <!-- Splice variable (spread array/list contents) -->

<!-- In attributes -->
<function name="@name"/>    <!-- @ prefix in attributes -->
```

### Parameter Definition vs Invocation
```xml
<!-- Definition: single colon -->
<:parameter/>               <!-- Define a parameter -->
<:rest.../>                 <!-- Define rest parameters -->
<:$>                        <!-- Define positional parameters -->

<!-- Invocation: double colon -->
<::parameter>value</::parameter>    <!-- Set named parameter -->
<::x>10</::x>                      <!-- Set x = 10 -->
```

### Positional Arguments
```xml
<!-- Positional argument definition -->
<:/>                        <!-- Anonymous positional parameter -->
<:.../>                     <!-- Collect remaining positional args -->

<!-- Accessing positional arguments -->
<@$1/>                      <!-- First positional arg -->
<@$2/>                      <!-- Second positional arg -->
<@$/>                       <!-- All positional args as list -->
<@...$/>                    <!-- Splice all positional args -->

<!-- Compact positional syntax -->
<add $:>1, 2, 3, 4</add>   <!-- Parse as positional args -->
<multiply $:=" ">5 10 2</multiply>  <!-- Custom separator -->
```

## Macro Definition

### Basic Macro Structure
```xml
<defmacro>
  <:macro-name>
    <:param1/>
    <:param2/>
    <:rest.../>
  </:macro-name>
  <!-- Macro body - returns AST -->
</defmacro>
```

### Parameter Types
```xml
<!-- Named parameters -->
<defmacro>
  <:when>
    <:test/>                <!-- Single named parameter -->
    <:body.../>             <!-- Rest parameter -->
  </:when>
  <if test="<@test/>">
    <progn><@body/></progn>
  </if>
</defmacro>

<!-- Positional parameters -->
<defmacro>
  <:add $:/>                <!-- Accepts positional args -->
  <+><@...$></+>
</defmacro>

<!-- Mixed parameters -->
<defmacro>
  <:between>
    <:low/>                 <!-- Named -->
    <:high/>                <!-- Named -->
    <:.../>                 <!-- Rest are positional -->
  </:between>
  <and>
    <gte><@$1/> <@low/></gte>
    <lte><@$1/> <@high/></lte>
  </and>
</defmacro>
```

### Macro Invocation
```xml
<!-- With named arguments -->
<when>
  <::test><gt><@x/> 5</gt></::test>
  <print>Greater than 5</print>
</when>

<!-- With positional arguments -->
<add>1, 2, 3, 4</add>
<add $:>1, 2, 3, 4</add>

<!-- Mixed invocation -->
<between>
  <::low>0</::low>
  <::high>100</::high>
  <:>42</:>
</between>
```

## AST Manipulation

### Mutation Operations (% prefix)
```xml
<!-- Removal -->
<%remove/>                  <!-- Remove matched element -->
<%remove-parent/>           <!-- Remove parent -->
<%empty/>                   <!-- Remove all children -->

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
<%unwrap/>                  <!-- Remove parent, keep element -->

<!-- Attributes -->
<%set-attr name="value"/>
<%remove-attr name="attr"/>
<%rename-attr from="old" to="new"/>
```

## Pattern Matching & Selectors

### CSS-Style Selectors
```xml
<!-- Basic selectors -->
function                    <!-- Tag name -->
function[name='main']       <!-- With attribute -->
function.public             <!-- With class -->
function#main              <!-- With ID -->

<!-- Hierarchical selectors -->
function > return           <!-- Direct child -->
function return             <!-- Any descendant -->
if + else                   <!-- Adjacent sibling -->
if ~ else                   <!-- Any following sibling -->

<!-- Pseudo-selectors -->
:first-child
:last-child
:nth-child(2)
:empty
:has(return)
:not(.private)
:within(function)
```

### Selector Combination with <-->
```xml
<!-- AND combination (all conditions must match) -->
<-->
  function,
  [visibility='public'],
  :not(:has(doc-comment)),
  :has(return),
</-->

<!-- OR combination -->
<or>
  <--> add > literal[value='0'] </-->
  <--> multiply > literal[value='1'] </-->
</or>
```

### Pattern Extraction
```xml
<!-- Extract values using $ variables -->
<select>
  <--> assign[target='$var'] > add > var[name='$var'], literal[value='1'] </-->
  <%replace-with>
    <increment><@$var/></increment>
  </%replace-with>
</select>

<!-- Extract multiple values -->
<select>
  <--> binary-op[op='$op'] > literal[value='$val'], var[name='$var'] </-->
  <!-- Now have $op, $val, and $var available -->
</select>
```

## Transformations

### Basic Transform Structure
```xml
<transform>
  <@ast/>                   <!-- Input AST -->

  <select>
    <--> selector </-->
    <%operation/>
  </select>

  <select>
    <--> another-selector </-->
    <%another-operation/>
  </select>
</transform>
```

### Pipeline Transformations
```xml
<pipeline>
  <@ast/>

  <!-- Stage 1: Find nodes -->
  <find-all>
    <--> function </-->
  </find-all>

  <!-- Stage 2: Filter -->
  <filter>
    <is-public/>
  </filter>

  <!-- Stage 3: Transform -->
  <map>
    <%add-logging/>
  </map>

  <!-- Stage 4: Apply changes -->
  <apply-changes/>
</pipeline>
```

### List Operations
```xml
<!-- Standard functional operations -->
<map>
  <@list/>
  <transform-function/>
</map>

<filter>
  <@list/>
  <predicate/>
</filter>

<reduce>
  <@list/>
  <initial>0</initial>
  <accumulator-function/>
</reduce>

<find>
  <@list/>
  <--> matching-selector </-->
</find>

<sort by="attribute-name">
  <@list/>
</sort>
```

## Compilation & Evaluation

### Compile-Time Operations
```xml
<!-- Compile-time evaluation -->
<compile-eval>
  <@expression/>
</compile-eval>

<!-- Conditional compilation -->
<compile-if test="condition">
  <then>...</then>
  <else>...</else>
</compile-if>

<!-- Compile-time let binding -->
<compile-let>
  <:variable><expression/></:variable>
  <!-- Use variable in expansion -->
</compile-let>

<!-- Generate unique symbols -->
<gensym prefix="temp"/>
```

### Runtime Invocation
```xml
<!-- Invoke stored AST -->
<invoke><@stored-ast/></invoke>

<!-- Invoke with environment -->
<invoke env="{x: 10, y: 20}">
  <@code/>
</invoke>
```

## Module System

### Import Directives
```xml
<!-- Import code/transforms -->
<import href="path/to/file.xml"/>
<import href="path/to/file.xml" as="namespace"/>

<!-- Conditional imports -->
<import href="debug.xml" when="development"/>

<!-- Pattern-based imports -->
<import-sources>
  <include pattern="src/**/*.xml"/>
  <exclude pattern="**/test/**"/>
</import-sources>
```

### File Structure
```xml
<!-- Any file can contain code, transforms, or both -->
<module>
  <!-- Code definitions -->
  <function name="helper">
    <!-- ... -->
  </function>

  <!-- Transform rules -->
  <select>
    <--> pattern </-->
    <%mutation/>
  </select>

  <!-- Imports -->
  <import href="other-module.xml"/>
</module>
```

### Transformation Stylesheets
```xml
<!-- standalone-transforms.style.xml -->
<stylesheet>
  <!-- Import files to transform -->
  <import-sources>
    <include pattern="src/**/*.code.xml"/>
  </import-sources>

  <!-- Define transformation rules -->
  <select>
    <--> console.* </-->
    <%remove/>
  </select>
</stylesheet>
```

## Complete Examples

### Example 1: Simple Macro
```xml
<defmacro>
  <:unless>
    <:condition/>
    <:body.../>
  </:unless>
  <if>
    <not><@condition/></not>
    <progn><@body/></progn>
  </if>
</defmacro>

<!-- Usage -->
<unless>
  <::condition><null><@user/></null></::condition>
  <process-user><@user/></process-user>
</unless>
```

### Example 2: Complex Transformation
```xml
<defmacro>
  <:optimize-function>
    <:fn/>
  </:optimize-function>

  <pipeline>
    <@fn/>

    <!-- Remove dead code -->
    <select>
      <--> if > condition > literal[value='false'] </-->
      <%replace-with select="else"/>
    </select>

    <!-- Optimize arithmetic -->
    <select>
      <--> add > literal[value='0'] </-->
      <%remove-parent/>
      <%keep select=":not(literal[value='0'])"/>
    </select>

    <!-- Add instrumentation -->
    <select>
      <--> function </-->
      <%prepend>
        <log>Entering <@name/></log>
      </%prepend>
    </select>
  </pipeline>
</defmacro>
```

### Example 3: DSL Creation
```xml
<defmacro>
  <:sql>
    <:query.../>
  </:sql>
  <compile-sql>
    <@query/>
  </compile-sql>
</defmacro>

<!-- Usage -->
<sql>
  <select>name, email</select>
  <from>users</from>
  <where>
    <and>
      <gt>age 18</gt>
      <eq>status 'active'</eq>
    </and>
  </where>
</sql>
```

### Example 4: Aspect-Oriented Transform
```xml
<!-- logging-aspect.xml -->
<stylesheet>
  <!-- Add logging to all public functions -->
  <select>
    <-->
      function[visibility='public'],
      :not(:has(> log)),
    </-->
    <%prepend>
      <log level="info">
        Calling <@name/> with args: <@arguments/>
      </log>
    </%prepend>
  </select>

  <!-- Add error handling to database operations -->
  <select>
    <--> database-query </-->
    <%wrap>
      <try>
        <%selected/>
        <catch>
          <::error/>
          <log level="error">Database error: <@error/></log>
          <rethrow/>
        </catch>
      </try>
    </%wrap>
  </select>
</stylesheet>
```
