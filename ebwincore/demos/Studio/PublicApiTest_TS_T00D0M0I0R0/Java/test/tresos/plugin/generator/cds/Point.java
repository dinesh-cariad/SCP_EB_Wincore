package test.tresos.plugin.generator.cds;

import dreisoft.tresos.generator.ng.api.cds.ctypes.SInt32;
import dreisoft.tresos.generator.ng.api.cds.ctypes.Struct;

/**
 * Struct Point defines an x and y value.
 *
 * <p>
 * <i> (C) Elektrobit Automotive GmbH - All rights reserved</i>
 * </p>
 *
 * @since 2008.b
 */
public class Point extends Struct
{

    /**
     * x of this point.
     *
     * It is safe to declare the members final. If you would change the instance after {@link #init()} was called the
     * member would loose its enclosing type!
     *
     * @since 2008.b
     */
    public final SInt32 x = new SInt32(0);

    /**
     * y of this point.
     *
     * @since 2008.b
     */
    public final SInt32 y = new SInt32(0);

    /**
     * Creates a new Point.
     *
     * This constructor is used if the instance is a member of an enclosing type or to generate a typedef of this class.
     *
     * @since 2008.b
     */
    public Point()
    {
        super();
        init(); // per convention call method init at end of constructor!
    }

    /**
     * Create a new Point using the given name as variable name. This constructor is used for standalone variables.
     *
     * @paran name the name of the point to create a global variable
     * @since 2008.b
     */
    public Point(String name)
    {
        super(name);
        init(); // per convention call method init at end of constructor!
    }

    /**
     * Of course, you can provide further constructors to initialize the values.
     *
     * @since 2008.b
     */
    public Point(long x, long y)
    {
        super();
        this.x.setValue(x);
        this.y.setValue(y);
        init(); // per convention call method init at end of constructor!
    }

    /**
     * Of course, you can provide further constructors to initialize the values.
     *
     * @paran name the name of the point to create a global variable
     * @since 2008.b
     */
    public Point(String name, long x, long y)
    {
        super(name);
        this.x.setValue(x);
        this.y.setValue(y);
        init(); // per convention call method init at end of constructor!
    }

} // Point

