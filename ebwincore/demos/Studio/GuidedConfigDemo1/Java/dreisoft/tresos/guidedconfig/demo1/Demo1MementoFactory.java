/*
 * Copyright (C) Elektrobit Automotive GmbH
 * All rights reserved
 */

package dreisoft.tresos.guidedconfig.demo1;

import dreisoft.tresos.guidedconfig.api.memento.AbstractMementoFactory;
import dreisoft.tresos.guidedconfig.api.memento.Memento;

/**
 * An Example for a custom MementoFactory.
 *
 * <p>
 * This demo implementation shows how to make only part of the memento attributes persistent.
 * </p>
 *
 * <p>
 * <i> Copyright (C) Elektrobit Automotive GmbH - All rights reserved</i>
 * </p>
 *
 * @since 2009.a
 */
public class Demo1MementoFactory extends AbstractMementoFactory
{

    // MementoIds which shall be persistent
    // the other mementos which are also conntected to GUI widgets but not persistent are not listed
    public static final String MEMENTOID_PAGE1_STRING = "Page1StringMemento";
    public static final String MEMENTOID_PAGE1_CHECK = "Page1CheckboxMemento";
    public static final String MEMENTOID_PAGE2_INTEGER = "Page2IntMemento";

    /**
     * Singleton instance of the factory
     */
    public static final Demo1MementoFactory INSTANCE = new Demo1MementoFactory();

    /**
     * Creates a MementoFactory
     */
    private Demo1MementoFactory()
    {
        super();
    }

    @Override
    public Memento getMemento(Memento parent, String name)
    {
        // let the superclass do its work
        Memento memento = super.getMemento(parent, name);

        // if this method is called with parent==null, then the root memento is to be created
        if (parent == null)
        {
            // root memento must be persistent, otherwise, nothing else will be persistent
            memento.setPersistent(true);

            String[] persistent_attrs = new String[]{
                MEMENTOID_PAGE1_CHECK,
                MEMENTOID_PAGE1_STRING,
                MEMENTOID_PAGE2_INTEGER};
            // set the memento attributes which should be stored persistently
            // the gui widgets will display the stored values if the editor is opened again
            memento.setPersistentAttributes(persistent_attrs);
        }
        return memento;
    }

} // MyMementoFactory

