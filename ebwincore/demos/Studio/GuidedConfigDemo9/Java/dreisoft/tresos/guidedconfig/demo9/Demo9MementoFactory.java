/*
 * Copyright (C) Elektrobit Automotive GmbH
 * All rights reserved
 */

package dreisoft.tresos.guidedconfig.demo9;

import dreisoft.tresos.guidedconfig.api.memento.AbstractMementoFactory;
import dreisoft.tresos.guidedconfig.api.memento.Memento;

/**
 * This special implementation of the memento factory sets part of the memento as not persistent.
 */
public class Demo9MementoFactory extends AbstractMementoFactory
{

    /**
     * Singleton instance of the factory
     */
    public static final Demo9MementoFactory INSTANCE = new Demo9MementoFactory();

    /**
     * Private default constructor for the singleton class.
     */
    private Demo9MementoFactory()
    {
        super();
    }

    @Override
    public Memento getMemento(Memento parent, String name)
    {
        // let the superclass do its work
        Memento memento = super.getMemento(parent, name);

        // make the memento for the table which is shown on the properties view not persistent
        // data is always taken from the treeTable depending on the current selection
        if ((name != null) && name.equals(IConstants.MEMENTO_KEY_TABLE))
        {
            memento.setPersistent(false);
        }

        return memento;
    }

}
