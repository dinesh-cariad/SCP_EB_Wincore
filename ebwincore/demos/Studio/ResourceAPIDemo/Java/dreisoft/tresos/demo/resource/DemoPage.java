/*
 * Copyright (C) Elektrobit Automotive GmbH
 * All rights reserved
 */

package dreisoft.tresos.demo.resource;

import dreisoft.tresos.demo.resource.generated.nls.ResourceAPIDemoNLS;
import dreisoft.tresos.guidedconfig.api.gui.WidgetFactory;
import dreisoft.tresos.guidedconfig.api.gui.WidgetFactory.TextFormat;
import dreisoft.tresos.guidedconfig.api.gui.form.SWTComboBox;
import dreisoft.tresos.guidedconfig.api.gui.page.AbstractPage;

import org.eclipse.swt.widgets.Composite;

/**
 * The page of the example runner wizard, consisting of a combobox for choosing an example an some text area for the
 * example description.
 *
 * <p>
 * <i> Copyright (C) Elektrobit Automotive GmbH - All rights reserved</i>
 * </p>
 *
 * @since 2009.a
 */
public class DemoPage extends AbstractPage
{

    public DemoPage()
    {
        super();
    }

    @Override
    public String doGetId()
    {
        return IConstants.PAGE_ID;
    }

    @Override
    public String doGetTitle()
    {
        return ResourceAPIDemoNLS.PAGE_TITLE();
    }

    @Override
    public String doGetDescription()
    {
        return ResourceAPIDemoNLS.PAGE_DESC();
    }

    @Override
    protected void doCreateControls(Composite parent)
    {

        WidgetFactory factory = getWidgetFactory();

        factory.createLabel(parent,
                            IConstants.WIDGET_ID_COMBO_EXAMPLE_LABEL,
                            IConstants.MEMENTO_KEY_COMBO_EXAMPLE_LABEL,
                            ResourceAPIDemoNLS.LABEL_COMBO_EXAMPLE());

        DemoBackend backend = (DemoBackend)getBackend();
        String[] range = backend.getExampleNames();

        SWTComboBox examples = factory.createCombo(parent,
                                                   IConstants.WIDGET_ID_COMBO_EXAMPLE,
                                                   IConstants.MEMENTO_KEY_COMBO_EXAMPLE,
                                                   TextFormat.TEXT,
                                                   range);

        // reset value in order to start sync to the description
        examples.getController().doSetValue(range[0]);

        factory.createBrowser(parent,
                              IConstants.WIDGET_ID_EXAMPLE_DESCRIPTION,
                              IConstants.MEMENTO_KEY_EXAMPLE_DESCRIPTION,
                              backend.getExampleDescription(range[0]),
                              0,
                              0,
                              500,
                              100,
                              true);

        factory.layout(parent);
    }
}
