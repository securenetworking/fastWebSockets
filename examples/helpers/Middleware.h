/* Middleware to fill out content-type */
inline bool hasExt(std::string_view file, std::string_view ext) {
    if (ext.size() > file.size()) {
        return false;
    }
    return std::equal(ext.rbegin(), ext.rend(), file.rbegin());
}

/* This should be a filter / middleware like app.use(handler) */
template <bool SSL>
fWS::HttpResponse<SSL> *serveFile(fWS::HttpResponse<SSL> *res, fWS::HttpRequest *req) {
    res->writeStatus(fWS::HTTP_200_OK);

    if (hasExt(req->getUrl(), ".svg")) {
        res->writeHeader("Content-Type", "image/svg+xml");
    }

    return res;
}
