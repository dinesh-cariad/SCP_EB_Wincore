/*
 * Copyright (C) EB Automotive
 * All rights reserved
 *
 * $Id: ConvertExample.java,v 1.2 2011/01/14 16:58:50 mast8006 Exp $
 */

package dreisoft.tresos.demo.resource.examples;

import dreisoft.tresos.autosar2.api.model.importer.ImporterConstants;
import dreisoft.tresos.datamodel2.api.model.Location;
import dreisoft.tresos.datamodel2.api.model.RCtxt;
import dreisoft.tresos.demo.resource.DemoBackend;
import dreisoft.tresos.demo.resource.DemoPlugin;
import dreisoft.tresos.demo.resource.generated.nls.ResourceAPIDemoNLS;
import dreisoft.tresos.demo.resource.generated.operationstatus.ResourceAPIDemoOperationStatus;
import dreisoft.tresos.lib2.api.log.APIOperationStatus;

import java.io.File;
import java.util.ArrayList;

/**
 * Example using the Resource API for converting a module configuration file (xdm) into AUTOSAR format (arxml).
 *
 * @since 2010.a
 */
public class ConvertExample extends ExampleBase
{

    /**
     * @param backend the demo backend that uses the example
     * @since 11
     */
    public ConvertExample(DemoBackend backend)
    {
        super(backend, ResourceAPIDemoNLS.DESCRIPTION_CONVERT_EXAMPLE());
    }

    /**
     * @since 2010.a
     */
    private static final String INFILE = "resources/convertExample/CanIf.xdm";

    /**
     * @since 2010.a
     */
    private static final String OUTFILE = "demoOutput/CanIf.arxml";

    /**
     * Does the conversion using the public Resource API.
     *
     * @param fileIn the input file handle to load
     * @param fileUriOut the file URI for the output
     * @param status where status information is added
     * @since 2010.a
     */
    private void doConvert(File fileIn, String fileUriOut, APIOperationStatus status)
    {
        // create a location from the input (XDM) file handle with content auto detection
        Location locationIn = new Location(fileIn);

        // create a new resource data model context node based on the file (initally empty and thus invalid)
        RCtxt rCtxt = RCtxt.create(locationIn);
        // load the content
        rCtxt.load(null, null, status);

        // set the new location
        Location locationOut = new Location(fileUriOut, ImporterConstants.CONTENT_TYPE_ASC + ":3.1");
        rCtxt.setLocation(locationOut);

        // save / convert
        rCtxt.save(null, null, status);
    }

    /**
     * Run the convert example.
     *
     * @param result to append status information onto
     * @throws Exception on any error
     * @since 2010.a
     */
    @Override
    public void run(ArrayList<APIOperationStatus> result) throws Exception
    {

        // prepare in and out file handles
        File inFile = DemoPlugin.getFileInBundle(INFILE);
        String outFileUri = getDemoBackend().getOutFileURI(OUTFILE);

        APIOperationStatus status = APIOperationStatus.getOkStatus();

        ///////////////////////////////
        // RESOURCE API IS USED HEREIN
        ///////////////////////////////
        doConvert(inFile, outFileUri, status);

        if (status.isOk())
        {
            // append some trace to be shown in the Results view
            result.add(ResourceAPIDemoOperationStatus.SEE_OUTPUT(outFileUri));

            // make results visible
            getDemoBackend().refreshECUConfigProject();
            getDemoBackend().openEditor(OUTFILE);
        }

        if (!status.isAbsolutelyOk())
        {
            result.add(status);
        }

    }

}
