Types
=====

Types in ``stapl``
------------------

``stapl`` has two kinds of types: basic types and tagged types. Basic types are
types that can be instantiated only with their names: ``int``, ``float`` and
``bool`` are good examples. On the other hand, tagged types are types that need
associated data -- they are called tags -- to instantiate. For instance, an
array of ``int`` with size 8 is written as ``array[int, 8]`` in ``stapl``.
Here, ``array[int, 8]`` needs tags ``int`` and ``8`` since both the type of
elements and the size determines the array's type. Thus, ``array[int, 8]`` is a
tagged type. For now, ``stapl`` has only two kinds of tagged types:
``array[T, N]`` and ``ptr[T]``.

.. doxygenfile:: types.h
