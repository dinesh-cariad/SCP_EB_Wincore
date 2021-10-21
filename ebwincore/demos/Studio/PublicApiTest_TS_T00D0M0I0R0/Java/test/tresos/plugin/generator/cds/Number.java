package test.tresos.plugin.generator.cds;

import dreisoft.tresos.generator.ng.api.cds.abstraction.CompilerAbstraction;
import dreisoft.tresos.generator.ng.api.cds.ctypes.SInt16;

/**
 * It is necessary to subclass built-in types to generate typedefs and variables of it (in this case the new type
 * "Number").
 *
 * <p>
 * <i> (C) Elektrobit Automotive GmbH - All rights reserved</i>
 * </p>
 *
 * @since 2008.b
 */
public class Number extends SInt16
{

    /**
     * Constructor only used for typedef.
     *
     * @since 2008.b
     */
    public Number()
    {
        super();
        init();
    }

    /**
     * Called by generator to create a number with a value.
     *
     * @param value The value
     * @since 2008.b
     */
    public Number(long value)
    {
        super(value);
        init();
    }

    /**
     * Create a number with a specific name to create a global variable.
     *
     * @param name The name of the variable
     * @param value The variable value
     * @since 2008.b
     */
    public Number(String name, long value)
    {
        super(name, value);
        init();
    }

    /**
     * Override this method to init the CType, e.g. set enclosingType of your fields.
     *
     * Adjusts the compiler abstraction.
     *
     * @see dreisoft.tresos.generator.ng.api.cds.ctypes.CType#init()
     * @since 2008.b
     */
    @Override
    protected final void init()
    {
        super.init(); // always call super.init()
        setCompilerAbstraction(new CompilerAbstraction(false));
    }

} // Number

