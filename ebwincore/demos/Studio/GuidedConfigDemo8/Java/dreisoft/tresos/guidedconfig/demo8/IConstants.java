/*
 * Copyright (C) Elektrobit Automotive GmbH
 * All rights reserved
 */

package dreisoft.tresos.guidedconfig.demo8;

import dreisoft.tresos.datamodel2.api.model.DCtxt;
import dreisoft.tresos.guidedconfig.demo8.backend.Demo8Backend;
import dreisoft.tresos.guidedconfig.demo8.pushservice.Demo8PullOperation;
import dreisoft.tresos.guidedconfig.demo8.pushservice.Demo8PushOperation;

/**
 * This interface contains constants used in several classed of this demo.
 */
public interface IConstants
{

    /**
     * Path to a parameter in the data model. This path points to the example parameter from the
     * <code>config/GCDemo8_TS_T00D0M0I0R0.xdm</code> file. It is used inside the two push operations
     * {@link Demo8PullOperation#doInvoke(dreisoft.tresos.guidedconfig.api.pushservice.AbstractPushEvent)} and
     * {@link Demo8PushOperation#doInvoke(dreisoft.tresos.guidedconfig.api.pushservice.AbstractPushEvent)} to read/write
     * the data model value from/to the {@link DCtxt} class.
     */
    public static final String DCTXT_PATH_STRING = "DataModel/ExampleValue";

    /**
     * This memento attribute stores the original value from the data model. Please note that it is not connected to any
     * GUI widget (and is thus not referenced in the file <code>pages/Pages.xml</code>). The value of the memento
     * attribute is set before the GUI page is displayed when
     * {@link Demo8PullOperation#doInvoke(dreisoft.tresos.guidedconfig.api.pushservice.AbstractPushEvent)} is called.
     */
    public static final String MEMENTO_ORIGINAL_TEXT = "/OriginalText";

    /**
     * This memento attribute stores the value which is written back into the data model when the editor is closed. It
     * is connected to a GUI widget, but the widget is not editable by the user. It is updated by the
     * {@link Demo8Backend#doHandleUpdatedData(java.util.List)} method.
     *
     * When the guided config wizard is closed, the value of this memento attribute is written back into the data model
     * by the method {@link Demo8PushOperation#doInvoke(dreisoft.tresos.guidedconfig.api.pushservice.AbstractPushEvent)}
     * .
     *
     * The memento attribute id is also referenced in the XForms page description in the file
     * <code>pages/Pages.xml</code>.
     */
    public static final String MEMENTO_PREVIEW_TEXT = "/PreviewText";

    /**
     * This memento attribute stores the "add prefix" boolean value which controls whether a prefix shall be added to
     * the text or not. Its value is used in the backend for updating the current value of {@link #MEMENTO_PREFIX_TEXT}.
     *
     * The memento attribute id is also referenced in the XForms page description in the file
     * <code>pages/Pages.xml</code>.
     */
    public static final String MEMENTO_ADD_PREFIX = "/AddPrefix";

    /**
     * This memento attribute stores the prefix value entered by the user.
     *
     * The memento attribute id is also referenced in the XForms page description in the file
     * <code>pages/Pages.xml</code>.
     */
    public static final String MEMENTO_PREFIX_TEXT = "/PrefixText";

}
