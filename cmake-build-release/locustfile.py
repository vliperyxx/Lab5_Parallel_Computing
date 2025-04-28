from locust import HttpUser, task

class WebsiteUser(HttpUser):
    @task
    def load_index(self):
        self.client.get("/index.html")

    @task
    def load_second_page(self):
        self.client.get("/second_page.html")

    @task
    def load_bad_request(self):
        self.client.get("/.../.../")

    @task
    def load_invalid_page(self):
        self.client.get("/nonexistent_page.html")
