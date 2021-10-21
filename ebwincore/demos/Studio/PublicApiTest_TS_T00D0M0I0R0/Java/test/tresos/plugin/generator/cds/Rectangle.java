package test.tresos.plugin.generator.cds;

import dreisoft.tresos.generator.ng.api.cds.ctypes.Struct;

/**
 * Struct Rectangle consists of two Points.
 *
 * <p>
 * <i> (C) Elektrobit Automotive GmbH - All rights reserved</i>
 * </p>
 *
 * @since 2008.b
 */
public class Rectangle extends Struct
{

    public final Point a;
    public final Point b;

    /**
     * Creates a new Rectangle.
     *
     * This constructor is used if the instance is a member of an enclosing type or to generate a typedef of this class.
     *
     * @since 2008.b
     */
    public Rectangle()
    {
        super();
        this.a = new Point();
        this.b = new Point();
        init();
    }

    /**
     * Creates a new Rectangle.
     *
     * @param name The variable name of the new instance.
     * @since 2008.b
     */
    public Rectangle(String name)
    {
        super(name);
        this.a = new Point();
        this.b = new Point();
        init();
    }

    /**
     * Creates a new Rectangle.
     *
     * @param name The variable name of the new instance.
     * @since 2008.b
     */
    public Rectangle(String name, Point a, Point b)
    {
        super(name);
        this.a = a;
        this.b = b;
        init();
    }

    /**
     * The init method can be implemented to initialize the members.
     *
     * @see dreisoft.tresos.generator.ng.api.cds.ctypes.Struct#init()
     * @since 2008.b
     */
    @Override
    protected final void init()
    {
        super.init(); // Don't forget to call super.init().

        this.a.setComment("This is point a.");
        this.b.setComment("This is point b.");
    }

} // Rectangle
